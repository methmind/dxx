# AngelScript Bindings -- Справочник

Документация по системе биндингов AngelScript в проекте **dxx (dota++)**.
Описывает архитектуру, все доступные API-модули и примеры использования из скриптов.

---

## Содержание

- [Обзор архитектуры](#обзор-архитектуры)
  - [Жизненный цикл скрипта](#жизненный-цикл-скрипта)
  - [Интерфейс C_IASBinding](#интерфейс-c_iasbinding)
  - [Порядок регистрации биндингов](#порядок-регистрации-биндингов)
  - [C_SharedPtr -- управление памятью](#c_sharedptr----управление-памятью)
- [Пространства имён (Namespaces)](#пространства-имён-namespaces)
- [imgui -- Типы ImGui](#imgui----типы-imgui)
  - [ImVec2](#imvec2)
  - [ImVec4](#imvec4)
  - [ImFont](#imfont)
  - [color32](#color32)
- [render -- Рендеринг](#render----рендеринг)
  - [C_Frame -- Рисование примитивов](#c_frame----рисование-примитивов)
    - [addLine](#addline)
    - [addText](#addtext)
    - [addFontText](#addfonttext)
    - [addRect](#addrect)
    - [addFilledRect](#addfilledrect)
    - [addTriangle](#addtriangle)
    - [addFilledTriangle](#addfilledtriangle)
    - [addCircle](#addcircle)
    - [addFilledCircle](#addfilledcircle)
    - [addImage](#addimage)
  - [Глобальные функции рендеринга](#глобальные-функции-рендеринга)
    - [measureText](#measuretext)
    - [loadVTEX](#loadvtex)
    - [loadPNG](#loadpng)
    - [loadFont](#loadfont)
    - [worldToScreen](#worldtoscreen)
- [hook -- Система хуков](#hook----система-хуков)
  - [hook_subscription_t](#hook_subscription_t)
  - [setOnRender](#setonrender)
  - [setOnLevelInit](#setonlevelinit)
  - [setOnLevelShutdown](#setonlevelshutdown)
- [gui -- GUI (зарезервировано)](#gui----gui-зарезервировано)
- [Встроенные аддоны AngelScript](#встроенные-аддоны-angelscript)
- [Полный пример скрипта](#полный-пример-скрипта)
- [Создание собственного биндинга (C++)](#создание-собственного-биндинга-c)

---

## Обзор архитектуры

### Жизненный цикл скрипта

Скрипты -- это файлы `.as`, расположенные в директории `%USERPROFILE%/dota++/`.  
Каждый скрипт может определить две точки входа:

```angelscript
void main()       // вызывается при загрузке скрипта
void onDispose()  // вызывается при выгрузке скрипта
```

Обе функции необязательны. Если `main()` отсутствует, скрипт загружается без выполнения стартовой логики. Если `onDispose()` отсутствует, при выгрузке не выполняется никакой финализации.

**Порядок исполнения:**

1. Скрипт компилируется через `CScriptBuilder`.
2. Создаётся экземпляр `C_ASInstance`, который находит функции `main()` и `onDispose()` в модуле.
3. `main()` вызывается немедленно после успешной компиляции.
4. При выгрузке скрипта (через UI или при завершении) вызывается `onDispose()`, после чего модуль сбрасывается (`Discard()`).

> **Исходный код:** `src/angelscript/as_instance.ixx`

---

### Интерфейс C_IASBinding

Все биндинги реализуют абстрактный интерфейс `C_IASBinding`:

```cpp
// src/angelscript/as_engine_binding_interface.ixx
class C_IASBinding
{
public:
    virtual ~C_IASBinding() = default;
    [[nodiscard]] virtual bool apply(const std::weak_ptr<C_IASEngine>& enginePtr) = 0;
};
```

Метод `apply()` вызывается один раз при регистрации биндинга в движке. Внутри него:

1. Получается `asIScriptEngine*` из `C_IASEngine`.
2. Устанавливается пространство имён через `SetDefaultNamespace()`.
3. Регистрируются типы, методы и глобальные функции через библиотеку **asbind20**.
4. Пространство имён сбрасывается обратно в `""`.

---

### Порядок регистрации биндингов

Биндинги регистрируются при инициализации приложения в функции `bootstrap::InitializeAngelscript()`:

```
src/bootstrap/bootstrap_angelscript.ixx
```

Порядок регистрации важен, так как некоторые биндинги зависят от типов, объявленных в других:

| # | Биндинг              | Зависимости                      |
|---|----------------------|----------------------------------|
| 1 | `C_ASBindingImgui`   | Нет                              |
| 2 | `C_ASBindingRenderer`| `imgui::ImVec2`, `imgui::ImVec4`, `imgui::ImFont` |
| 3 | `C_ASBindingHook`    | `render::C_Frame`                |

Каждый биндинг добавляется вызовом `asEngine->addBinding(std::make_unique<...>())`:

```cpp
// src/angelscript/as_engine.ixx:68-76
[[nodiscard]] bool addBinding(std::unique_ptr<C_IASBinding> binding)
{
    if (!binding->apply(weak_from_this())) {
        return false;
    }
    this->bindings_.push_back(std::move(binding));
    return true;
}
```

---

### C_SharedPtr -- управление памятью

```
src/angelscript/binding/memory/as_binding_memory.ixx
```

`C_SharedPtr<T>` -- обёртка с интрузивным подсчётом ссылок поверх `std::shared_ptr<T>`, предназначенная для корректной работы с reference-типами AngelScript.

**Ключевые свойства:**

- Начальный `refCount = 1` при создании через `new`.
- `addRef()` / `release()` -- атомарные операции подсчёта ссылок.
- Деструктор приватный -- объект может быть создан только через `new` и освобождён через `release()`.
- Копирование и перемещение запрещены.
- `implicitCast<base_t>()` -- безопасное приведение типов через `dynamic_pointer_cast` с передачей владения.

Этот класс используется в биндингах для типов `hook_subscription_t` и `d3d_texture_t`, где AngelScript управляет временем жизни объектов через хэндлы (`@`).

---

## Пространства имён (Namespaces)

Все API-функции и типы скрипта организованы по пространствам имён:

| Namespace | Модуль                         | Описание                               |
|-----------|--------------------------------|----------------------------------------|
| `imgui`   | `as.binding.imgui`             | Типы ImGui (векторы, цвета, шрифты)    |
| `render`  | `as.binding.renderer`          | Рисование, загрузка ресурсов, проекция |
| `hook`    | `as.binding.hook`              | Подписка на события движка             |
| `gui`     | `as.binding.gui`               | Зарезервировано (пока пусто)           |

Из скрипта доступ осуществляется через синтаксис `namespace::func()` или `namespace::Type`.

---

## imgui -- Типы ImGui

> **Исходный код:** `src/angelscript/binding/as_binding_imgui.ixx`

Пространство имён `imgui` экспортирует базовые типы Dear ImGui, которые используются как параметры в функциях рендеринга.

---

### ImVec2

2D-вектор (значение, POD-тип).

```angelscript
imgui::ImVec2 v(100.0f, 200.0f);
float x = v.x;
float y = v.y;
```

| Конструктор | Описание |
|-------------|----------|
| `ImVec2(float x, float y)` | Создаёт вектор с указанными координатами |

| Свойство | Тип   | Описание |
|----------|-------|----------|
| `x`      | float | Координата X |
| `y`      | float | Координата Y |

---

### ImVec4

4D-вектор (значение, POD-тип). Используется для задания прямоугольников (`x, y, z, w` = `x1, y1, x2, y2`) и цветов.

```angelscript
imgui::ImVec4 rect(10.0f, 20.0f, 200.0f, 150.0f);
```

| Конструктор | Описание |
|-------------|----------|
| `ImVec4(float x, float y, float z, float w)` | Создаёт 4D-вектор |

| Свойство | Тип   | Описание |
|----------|-------|----------|
| `x`      | float | Компонента X (или left)   |
| `y`      | float | Компонента Y (или top)    |
| `z`      | float | Компонента Z (или right)  |
| `w`      | float | Компонента W (или bottom) |

---

### ImFont

Ссылочный тип (handle, `@`). Представляет загруженный шрифт. Получается через `render::loadFont()`.

```angelscript
imgui::ImFont@ font = render::loadFont("C:/fonts/arial.ttf", 16.0f);
float size = font.getFontSize();
```

| Метод | Возврат | Описание |
|-------|---------|----------|
| `getFontSize()` | `float` | Возвращает размер шрифта, с которым он был запечён (baked) |

> **Примечание:** `ImFont` зарегистрирован с флагом `asOBJ_NOCOUNT` -- AngelScript не управляет его временем жизни. Шрифты живут пока существует ImGui-контекст.

---

### color32

Глобальная функция для создания 32-битного цвета в формате ABGR (стандарт ImGui).

```angelscript
uint red   = imgui::color32(255, 0, 0, 255);
uint green = imgui::color32(0, 255, 0, 255);
uint semiTransparentWhite = imgui::color32(255, 255, 255, 128);
```

| Сигнатура | Возврат |
|-----------|---------|
| `uint color32(uint8 r, uint8 g, uint8 b, uint8 a)` | `uint` (32-bit цвет) |

| Параметр | Тип    | Описание              |
|----------|--------|-----------------------|
| `r`      | uint8  | Красный канал (0-255) |
| `g`      | uint8  | Зелёный канал (0-255) |
| `b`      | uint8  | Синий канал (0-255)   |
| `a`      | uint8  | Альфа-канал (0-255)   |

---

## render -- Рендеринг

> **Исходный код:** `src/angelscript/binding/as_binding_renderer.ixx`

Пространство имён `render` предоставляет API для рисования примитивов, загрузки текстур/шрифтов и математических утилит.

---

### C_Frame -- Рисование примитивов

`render::C_Frame` -- обёртка над `ImDrawList`. Получается как аргумент колбэка `hook::setOnRender()`. Предоставляет методы для рисования на overlay поверх игры.

> **Важно:** Все методы `C_Frame` должны вызываться **только** внутри колбэка рендера. Вызов вне контекста рендера приведёт к неопределённому поведению.

---

#### addLine

Рисует линию между двумя точками.

```angelscript
void addLine(imgui::ImVec2 start, imgui::ImVec2 end, uint color, float thickness)
```

| Параметр    | Тип          | Описание                    |
|-------------|--------------|------------------------------|
| `start`     | `ImVec2`     | Начальная точка              |
| `end`       | `ImVec2`     | Конечная точка               |
| `color`     | `uint`       | Цвет (через `color32`)      |
| `thickness` | `float`      | Толщина линии в пикселях     |

```angelscript
frame.addLine(imgui::ImVec2(0, 0), imgui::ImVec2(100, 100), imgui::color32(255, 0, 0, 255), 2.0f);
```

---

#### addText

Рисует текст шрифтом по умолчанию.

```angelscript
void addText(const string& in text, imgui::ImVec2 pos, uint color)
```

| Параметр | Тип      | Описание                       |
|----------|----------|--------------------------------|
| `text`   | `string` | Текст для отображения          |
| `pos`    | `ImVec2` | Позиция (левый верхний угол)   |
| `color`  | `uint`   | Цвет текста                    |

```angelscript
frame.addText("Hello, World!", imgui::ImVec2(50, 50), imgui::color32(255, 255, 255, 255));
```

---

#### addFontText

Рисует текст указанным шрифтом и размером.

```angelscript
void addFontText(const string& in text, imgui::ImFont@ font, float fontSize, imgui::ImVec2 pos, uint color)
```

| Параметр   | Тип       | Описание                                      |
|------------|-----------|------------------------------------------------|
| `text`     | `string`  | Текст для отображения                          |
| `font`     | `ImFont@` | Шрифт (из `loadFont`). Если `null` или невалидный -- используется шрифт по умолчанию |
| `fontSize` | `float`   | Размер шрифта для рендера                      |
| `pos`      | `ImVec2`  | Позиция (левый верхний угол)                   |
| `color`    | `uint`    | Цвет текста                                    |

```angelscript
imgui::ImFont@ myFont = render::loadFont("C:/fonts/custom.ttf", 24.0f);

void onRender(render::C_Frame@ frame)
{
    frame.addFontText("Custom font!", myFont, 24.0f, imgui::ImVec2(100, 100),
        imgui::color32(0, 255, 0, 255));
}
```

---

#### addRect

Рисует контур прямоугольника.

```angelscript
void addRect(imgui::ImVec4 bounds, uint color, float rounding, float thickness)
```

| Параметр    | Тип      | Описание                                      |
|-------------|----------|------------------------------------------------|
| `bounds`    | `ImVec4` | Границы: `(x1, y1, x2, y2)`                   |
| `color`     | `uint`   | Цвет контура                                   |
| `rounding`  | `float`  | Скругление углов (0 = без скругления)           |
| `thickness` | `float`  | Толщина линии контура                           |

```angelscript
frame.addRect(imgui::ImVec4(10, 10, 200, 100), imgui::color32(255, 255, 0, 255), 5.0f, 2.0f);
```

---

#### addFilledRect

Рисует залитый прямоугольник.

```angelscript
void addFilledRect(imgui::ImVec4 bounds, uint color, float rounding)
```

| Параметр   | Тип      | Описание                                      |
|------------|----------|------------------------------------------------|
| `bounds`   | `ImVec4` | Границы: `(x1, y1, x2, y2)`                   |
| `color`    | `uint`   | Цвет заливки                                   |
| `rounding` | `float`  | Скругление углов (0 = без скругления)           |

```angelscript
frame.addFilledRect(imgui::ImVec4(10, 10, 200, 100), imgui::color32(0, 0, 0, 150), 0.0f);
```

---

#### addTriangle

Рисует контур треугольника.

```angelscript
void addTriangle(imgui::ImVec2 a, imgui::ImVec2 b, imgui::ImVec2 c, uint color, float thickness)
```

| Параметр    | Тип      | Описание        |
|-------------|----------|-----------------|
| `a`         | `ImVec2` | Первая вершина  |
| `b`         | `ImVec2` | Вторая вершина  |
| `c`         | `ImVec2` | Третья вершина  |
| `color`     | `uint`   | Цвет контура    |
| `thickness` | `float`  | Толщина линии   |

---

#### addFilledTriangle

Рисует залитый треугольник.

```angelscript
void addFilledTriangle(imgui::ImVec2 a, imgui::ImVec2 b, imgui::ImVec2 c, uint color)
```

| Параметр | Тип      | Описание        |
|----------|----------|-----------------|
| `a`      | `ImVec2` | Первая вершина  |
| `b`      | `ImVec2` | Вторая вершина  |
| `c`      | `ImVec2` | Третья вершина  |
| `color`  | `uint`   | Цвет заливки    |

---

#### addCircle

Рисует контур окружности.

```angelscript
void addCircle(imgui::ImVec2 center, float radius, uint color, int segments, float thickness)
```

| Параметр   | Тип      | Описание                                     |
|------------|----------|----------------------------------------------|
| `center`   | `ImVec2` | Центр окружности                             |
| `radius`   | `float`  | Радиус                                       |
| `color`    | `uint`   | Цвет контура                                 |
| `segments` | `int`    | Количество сегментов (0 = автоматический выбор) |
| `thickness`| `float`  | Толщина линии                                |

```angelscript
frame.addCircle(imgui::ImVec2(400, 300), 50.0f, imgui::color32(0, 255, 255, 255), 0, 1.5f);
```

---

#### addFilledCircle

Рисует залитую окружность.

```angelscript
void addFilledCircle(imgui::ImVec2 center, float radius, uint color, int segments)
```

| Параметр   | Тип      | Описание                                     |
|------------|----------|----------------------------------------------|
| `center`   | `ImVec2` | Центр окружности                             |
| `radius`   | `float`  | Радиус                                       |
| `color`    | `uint`   | Цвет заливки                                 |
| `segments` | `int`    | Количество сегментов (0 = автоматический выбор) |

---

#### addImage

Рисует текстуру (изображение) в указанной области.

```angelscript
void addImage(const render::d3d_texture_t& in texture, imgui::ImVec2 pos, imgui::ImVec2 size, uint color)
```

| Параметр  | Тип              | Описание                                               |
|-----------|------------------|---------------------------------------------------------|
| `texture` | `d3d_texture_t@` | Загруженная текстура (из `loadVTEX` или `loadPNG`)     |
| `pos`     | `ImVec2`         | Позиция (левый верхний угол)                            |
| `size`    | `ImVec2`         | Размер области вывода                                   |
| `color`   | `uint`           | Тинт-цвет (белый = без тинта: `color32(255,255,255,255)`) |

```angelscript
render::d3d_texture_t@ tex = render::loadPNG("C:/images/icon.png");

void onRender(render::C_Frame@ frame)
{
    frame.addImage(tex, imgui::ImVec2(10, 10), imgui::ImVec2(64, 64),
        imgui::color32(255, 255, 255, 255));
}
```

> **Исключение:** Если текстура `null` или невалидна, будет выброшено скриптовое исключение `"Texture couldnt be a nullptr!"`.

---

### Глобальные функции рендеринга

Эти функции доступны напрямую в пространстве имён `render`.

---

#### measureText

Вычисляет размер текста в пикселях шрифтом по умолчанию.

```angelscript
imgui::ImVec2 render::measureText(const string& in text)
```

| Параметр | Тип      | Описание           |
|----------|----------|--------------------|
| `text`   | `string` | Текст для измерения |

| Возврат  | Описание                      |
|----------|-------------------------------|
| `ImVec2` | Ширина и высота текста в px   |

```angelscript
imgui::ImVec2 sz = render::measureText("Hello");
print("Width: " + sz.x + ", Height: " + sz.y);
```

---

#### loadVTEX

Загружает текстуру из Valve VTEX-файла (форматы DXT1, DXT5, RGBA8888, PNG, LZ4, YCoCg).

```angelscript
render::d3d_texture_t@ render::loadVTEX(const string& in path)
```

| Параметр | Тип      | Описание                                  |
|----------|----------|-------------------------------------------|
| `path`   | `string` | Путь к `.vtex_c` файлу                    |

| Возврат          | Описание                                          |
|------------------|---------------------------------------------------|
| `d3d_texture_t@` | Handle на загруженную текстуру, или `null` при ошибке |

> **Исключение:** При неудачной загрузке выбрасывается скриптовое исключение `"Failed to load image: <path>"`.

---

#### loadPNG

Загружает текстуру из PNG-файла.

```angelscript
render::d3d_texture_t@ render::loadPNG(const string& in path)
```

| Параметр | Тип      | Описание              |
|----------|----------|-----------------------|
| `path`   | `string` | Путь к `.png` файлу   |

| Возврат          | Описание                                          |
|------------------|---------------------------------------------------|
| `d3d_texture_t@` | Handle на загруженную текстуру, или `null` при ошибке |

> **Исключение:** При неудачной загрузке выбрасывается скриптовое исключение `"Failed to load image: <path>"`.

> **Примечание:** Текстуры `d3d_texture_t` управляются подсчётом ссылок через `C_SharedPtr`. Когда все хэндлы в скрипте освобождаются, текстура автоматически выгружается из GPU.

---

#### loadFont

Загружает TTF-шрифт с диска и регистрирует его в ImGui.

```angelscript
imgui::ImFont@ render::loadFont(const string& in path, float size)
```

| Параметр | Тип      | Описание                            |
|----------|----------|--------------------------------------|
| `path`   | `string` | Путь к `.ttf` файлу                 |
| `size`   | `float`  | Размер шрифта в пикселях             |

| Возврат    | Описание                                          |
|------------|---------------------------------------------------|
| `ImFont@`  | Загруженный шрифт, или `null` при ошибке          |

> **Примечание:** Поддерживаются кириллические глифы (`GetGlyphRangesCyrillic()`). После загрузки шрифта происходит пересоздание ресурсов DX11 (атласа шрифтов), что может вызвать кратковременный артефакт. Рекомендуется загружать шрифты в `main()`.

```angelscript
imgui::ImFont@ titleFont;

void main()
{
    @titleFont = render::loadFont("C:/Windows/Fonts/arial.ttf", 32.0f);
}
```

---

#### worldToScreen

Проецирует 3D-координату из мирового пространства в 2D-координату на экране.

```angelscript
imgui::ImVec2 render::worldToScreen(float x, float y, float z)
```

| Параметр | Тип    | Описание                |
|----------|--------|--------------------------|
| `x`      | float  | Мировая координата X     |
| `y`      | float  | Мировая координата Y     |
| `z`      | float  | Мировая координата Z     |

| Возврат  | Описание                                                     |
|----------|--------------------------------------------------------------|
| `ImVec2` | Экранные координаты. `(-1, -1)` если точка за пределами экрана |

```angelscript
imgui::ImVec2 screenPos = render::worldToScreen(100.0f, 200.0f, 50.0f);
if (screenPos.x >= 0 && screenPos.y >= 0)
{
    frame.addText("Target", screenPos, imgui::color32(255, 0, 0, 255));
}
```

---

## hook -- Система хуков

> **Исходный код:** `src/angelscript/binding/as_binding_hook.ixx`

Пространство имён `hook` предоставляет подписку на события движка. Каждая функция подписки возвращает `hook_subscription_t@` -- RAII-хэндл подписки. Пока хэндл жив, колбэк вызывается при возникновении события. Когда хэндл освобождается (или скрипт выгружается), подписка автоматически отменяется.

---

### hook_subscription_t

Ссылочный тип -- хэндл подписки на хук. Управляется подсчётом ссылок.

```angelscript
hook::hook_subscription_t@ sub = hook::setOnRender(onRender);
```

**Поведение:**
- Пока переменная `sub` жива -- колбэк вызывается.
- Установка `@sub = null` или выход переменной из области видимости -> подписка отменяется.
- Если вы хотите, чтобы подписка жила всё время работы скрипта, храните хэндл в глобальной переменной.

---

### setOnRender

Подписка на событие рендера. Колбэк вызывается каждый кадр при отрисовке overlay.

```angelscript
hook::hook_subscription_t@ hook::setOnRender(onRender@ callback)
```

**Сигнатура колбэка:**

```angelscript
funcdef void onRender(render::C_Frame@ frame);
```

| Параметр   | Тип             | Описание                               |
|------------|-----------------|----------------------------------------|
| `callback` | `onRender@`     | Функция, вызываемая каждый кадр        |

Колбэк получает `render::C_Frame@` -- объект для рисования примитивов.

```angelscript
hook::hook_subscription_t@ renderSub;

void main()
{
    @renderSub = hook::setOnRender(onRender);
}

void onRender(render::C_Frame@ frame)
{
    frame.addText("Rendering!", imgui::ImVec2(10, 10), imgui::color32(255, 255, 255, 255));
}
```

> **Исключение:** Если `callback` равен `null`, выбрасывается скриптовое исключение `"Callback function cannot be null!"`.

---

### setOnLevelInit

Подписка на событие инициализации уровня (загрузка карты).

```angelscript
hook::hook_subscription_t@ hook::setOnLevelInit(onLevelInit@ callback)
```

**Сигнатура колбэка:**

```angelscript
funcdef void onLevelInit();
```

```angelscript
hook::hook_subscription_t@ levelSub;

void main()
{
    @levelSub = hook::setOnLevelInit(onLevel);
}

void onLevel()
{
    print("Level loaded!");
}
```

---

### setOnLevelShutdown

Подписка на событие завершения уровня (выгрузка карты).

```angelscript
hook::hook_subscription_t@ hook::setOnLevelShutdown(onLevelShutdown@ callback)
```

**Сигнатура колбэка:**

```angelscript
funcdef void onLevelShutdown();
```

```angelscript
hook::hook_subscription_t@ shutdownSub;

void main()
{
    @shutdownSub = hook::setOnLevelShutdown(onShutdown);
}

void onShutdown()
{
    print("Level unloaded!");
}
```

---

## gui -- GUI (зарезервировано)

> **Исходный код:** `src/angelscript/binding/as_binding_gui.ixx`

Пространство имён `gui` зарегистрировано, но на данный момент **не содержит биндингов**. Предполагается, что в будущем здесь появятся функции для программного создания виджетов GUI из скриптов.

---

## Встроенные аддоны AngelScript

Помимо пользовательских биндингов, движок регистрирует стандартные аддоны AngelScript при инициализации (`as_engine.ixx:100-113`):

| Аддон               | Описание                                        |
|----------------------|-------------------------------------------------|
| `array<T>`          | Динамические массивы                             |
| `string`            | Строковый тип и утилиты                          |
| `dictionary`        | Ассоциативный контейнер (ключ-значение)          |
| `math`              | Математические функции (`sin`, `cos`, `sqrt` и т.д.) |
| `file`              | Чтение/запись файлов                             |
| `any`               | Контейнер для произвольного типа                 |
| `ref`               | Обобщённый ссылочный хэндл                       |
| `socket`            | Сетевые сокеты                                   |
| `datetime`          | Дата и время                                     |

Также зарегистрирована глобальная функция:

```angelscript
void print(const string& in msg)  // выводит сообщение в debug-лог
```

---

## Полный пример скрипта

```angelscript
// example.as -- Полный пример скрипта для dxx
// Поместите в %USERPROFILE%/dota++/

// Глобальные переменные для хранения подписок и ресурсов
hook::hook_subscription_t@ renderSub;
hook::hook_subscription_t@ levelInitSub;
hook::hook_subscription_t@ levelShutdownSub;

imgui::ImFont@ customFont;
render::d3d_texture_t@ iconTexture;

bool isInGame = false;

void main()
{
    print("Script loaded!");

    // Загрузка ресурсов (лучше делать здесь, а не в onRender)
    @customFont = render::loadFont("C:/Windows/Fonts/arial.ttf", 18.0f);
    @iconTexture = render::loadPNG("C:/images/icon.png");

    // Подписка на события
    @renderSub = hook::setOnRender(onRender);
    @levelInitSub = hook::setOnLevelInit(onLevelInit);
    @levelShutdownSub = hook::setOnLevelShutdown(onLevelShutdown);
}

void onDispose()
{
    print("Script unloaded!");
    // Подписки автоматически отменяются при освобождении хэндлов,
    // но можно отменить явно:
    @renderSub = null;
    @levelInitSub = null;
    @levelShutdownSub = null;
}

void onRender(render::C_Frame@ frame)
{
    // Заголовок
    frame.addFontText("dxx overlay", customFont, 18.0f,
        imgui::ImVec2(10, 10), imgui::color32(0, 200, 255, 255));

    // Фон под текстом
    imgui::ImVec2 textSize = render::measureText("dxx overlay");
    frame.addFilledRect(
        imgui::ImVec4(8, 8, 12 + textSize.x, 12 + textSize.y),
        imgui::color32(0, 0, 0, 120), 4.0f
    );

    // Иконка
    if (iconTexture !is null)
    {
        frame.addImage(iconTexture, imgui::ImVec2(10, 40),
            imgui::ImVec2(32, 32), imgui::color32(255, 255, 255, 255));
    }

    // Индикатор состояния
    uint stateColor = isInGame
        ? imgui::color32(0, 255, 0, 255)
        : imgui::color32(255, 0, 0, 255);
    frame.addFilledCircle(imgui::ImVec2(30, 90), 8.0f, stateColor, 0);

    // Перекрестие в центре (пример worldToScreen)
    imgui::ImVec2 w2s = render::worldToScreen(0.0f, 0.0f, 0.0f);
    if (w2s.x >= 0)
    {
        uint crossColor = imgui::color32(255, 255, 0, 200);
        frame.addLine(
            imgui::ImVec2(w2s.x - 10, w2s.y),
            imgui::ImVec2(w2s.x + 10, w2s.y),
            crossColor, 1.0f
        );
        frame.addLine(
            imgui::ImVec2(w2s.x, w2s.y - 10),
            imgui::ImVec2(w2s.x, w2s.y + 10),
            crossColor, 1.0f
        );
    }
}

void onLevelInit()
{
    isInGame = true;
    print("Level initialized!");
}

void onLevelShutdown()
{
    isInGame = false;
    print("Level shutdown!");
}
```

---

## Создание собственного биндинга (C++)

Чтобы добавить новый биндинг в движок:

### 1. Создайте модуль (.ixx)

```cpp
// src/angelscript/binding/as_binding_example.ixx
module;
#include <memory>
#include "asbind20/asbind.hpp"

export module as.binding.example;

import as.binding;
import as.engine_interface;

namespace as
{
    constexpr auto AS_EXAMPLE_NAMESPACE_NAME = "example";

    export class C_ASBindingExample : public C_IASBinding
    {
    public:
        [[nodiscard]] bool apply(const std::weak_ptr<C_IASEngine>& engineWeak) override
        {
            const auto enginePtr = engineWeak.lock();
            if (!enginePtr) {
                return false;
            }

            const auto engine = enginePtr->getEngine();
            engine->SetDefaultNamespace(AS_EXAMPLE_NAMESPACE_NAME);

            // Регистрация типов и функций через asbind20
            asbind20::global(engine).function(
                "int add(int, int)",
                [](int a, int b) { return a + b; }
            );

            engine->SetDefaultNamespace("");
            return true;
        }
    };
}
```

### 2. Зарегистрируйте биндинг в bootstrap

В файле `src/bootstrap/bootstrap_angelscript.ixx` добавьте:

```cpp
import as.binding.example;

// Внутри InitializeAngelscript():
if (!asEngine->addBinding(std::make_unique<as::C_ASBindingExample>())) {
    dbg("Unable to initialize example binding!");
    return false;
}
```

### 3. Используйте из скрипта

```angelscript
void main()
{
    int result = example::add(2, 3);
    print("2 + 3 = " + result);
}
```

### Правила регистрации

- **Порядок важен:** Если ваш биндинг использует типы из другого биндинга (например, `imgui::ImVec2`), он должен быть зарегистрирован **после** него.
- **Namespace:** Всегда устанавливайте `SetDefaultNamespace` перед регистрацией и сбрасывайте в `""` после.
- **Reference-типы:** Для типов со счётчиком ссылок используйте `C_SharedPtr<T>` и регистрируйте `addref`/`release` через `asbind20::ref_class`.
- **Value-типы:** Для POD-типов используйте `asbind20::value_class` с соответствующими флагами (`asOBJ_POD`, `asOBJ_APP_CLASS_ALLFLOATS` и т.д.).
- **Ошибки из колбэков:** Используйте `asbind20::set_script_exception()` для выброса скриптовых исключений.

---

## Файловая структура

```
src/angelscript/binding/
├── as_binding_gui.ixx          # gui:: (зарезервировано)
├── as_binding_hook.ixx         # hook:: (подписки на события)
├── as_binding_imgui.ixx        # imgui:: (типы ImVec2, ImVec4, ImFont, color32)
├── as_binding_renderer.ixx     # render:: (C_Frame, загрузка ресурсов, worldToScreen)
└── memory/
    └── as_binding_memory.ixx   # C_SharedPtr<T> (управление памятью для ref-типов)
```

**Связанные файлы:**

| Файл | Описание |
|------|----------|
| `src/angelscript/as_engine_binding_interface.ixx` | Интерфейс `C_IASBinding` |
| `src/angelscript/as_engine_interface.ixx` | Интерфейс `C_IASEngine` |
| `src/angelscript/as_engine.ixx` | Реализация движка, `addBinding()` |
| `src/angelscript/as_manager.ixx` | Менеджер скриптов (загрузка/выгрузка) |
| `src/angelscript/as_instance.ixx` | Экземпляр скрипта (main/onDispose) |
| `src/bootstrap/bootstrap_angelscript.ixx` | Инициализация всех биндингов |
| `src/hook/hook_dispatcher.ixx` | Диспетчер хуков (C++ сторона) |
| `src/hook/impl/hook_impl_type.ixx` | Перечисление `hook_type_e` |
