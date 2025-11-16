//
// Created by sexey on 16.11.2025.
//

#ifndef DXX_DLC_SDK_CLIENT_FRAME_STAGE_T_H
#define DXX_DLC_SDK_CLIENT_FRAME_STAGE_T_H

namespace sdk
{
    typedef enum {
        UNDEFINED = -1,                           // Не выполнялись кадры (инициализация)
        START = 0,                                // Начало кадра
        NET_UPDATE_START = 1,                     // Начало получения сетевого пакета
        NET_UPDATE_POSTDATAUPDATE_START = 2,      // Начало обработки полученных данных
        NET_UPDATE_POSTDATAUPDATE_END = 3,        // Завершение обработки данных
        NET_UPDATE_END = 4,                       // Завершение сетевого обновления
        RENDER_START = 5,                         // Начало рендеринга
        RENDER_END = 6,                           // Завершение рендеринга
        // Source 2 специфичные расширения
        UNKNOWN_7 = 7,                            // Промежуточная стадия (новая в Source 2)
        // Пропущено 8
        UNKNOWN_9 = 9,                            // Вызывается перед START (новая в Source 2)
        UNKNOWN_10 = 10                           // Частые вызовы (новая в Source 2)
    } ClientFrameStage_t;
}

#endif //DXX_DLC_SDK_CLIENT_FRAME_STAGE_T_H