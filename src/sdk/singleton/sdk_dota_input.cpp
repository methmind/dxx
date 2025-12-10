//
// Created by sexey on 10.12.2025.
//

#include "sdk_dota_input.h"
#include "debug/debug_output.h"
#include "memory/pattern_scanner.h"
#include "minhook/src/hde/hde64.h"
#include "sdk/sdk_signature.h"

namespace sdk::singleton
{
    bool C_DotaInput::resolveFunctions(HMODULE clientModule)
    {
        if (this->createMove_ = reinterpret_cast<create_move_t>(
            memory::FindPattern(clientModule, signature::CINPUT_CREATE_MOVE_FUNC)); !this->createMove_) {
            dbg("Unable to find C_DotaInput::CreateMove function!");
            return false;
        }

        if (this->getScreenCmdBuffer_ = reinterpret_cast<get_screen_cmd_buffer_t>(
            memory::FindPattern(clientModule, signature::GET_SCREEN_CMD_BUFFER_FUNC)); !this->getScreenCmdBuffer_) {
            dbg("Unable to find C_DotaInput::GetScreenCmdBuffer function!");
            return false;
        }

        if (this->getUserCmd_ = reinterpret_cast<get_user_cmd_t>(
            memory::FindPattern(clientModule, signature::CINPUT_GET_USER_CMD_FUNC)); !this->getUserCmd_) {
            dbg("Unable to find C_DotaInput::GetUserCmd function!");
            return false;
        }

        if (this->getScreenPlayerController_ = reinterpret_cast<get_screen_player_controller_t>(
            memory::FindPattern(clientModule, signature::GET_SCREEN_PLAYER_CONTROLLER_FUNC));
            this->getScreenPlayerController_) {
            dbg("Unable to find ::GetScreenPlayerController function!");
            return false;
        }

        return true;
    }

    bool C_DotaInput::resolveGlobalCmdBuffer(HMODULE clientModule)
    {
        const auto movRcx = static_cast<uint8_t*>(
            memory::FindPattern(clientModule, signature::MOV_RCX_GLOBAL_CMD_CIRCULAR_BUFFER)
        );

        if (!movRcx) {
            dbg("Unable to find \"mov rcx, global cmd circular buffer\" pattern!");
            return false;
        }

        hde64s hs;
        const auto length = hde64_disasm(movRcx, &hs);
        if (!length || hs.flags & F_ERROR) {
            dbg("Unable to disasm \"mov rcx, cs:g_UserCmd\"");
            return false;
        }

        this->cmdCircularBuffer_ = *reinterpret_cast<void**>(movRcx + hs.disp.disp32 + length);
        dbg("g_UserCmd ptr: %p", this->cmdCircularBuffer_);

        return true;
    }

    bool C_DotaInput::resolveSequenceNumberOffset(HMODULE clientModule)
    {
        const auto movR14d = static_cast<uint8_t*>(
            memory::FindPattern(clientModule, signature::ADD_RAX_SEQUENCE_NUMBER_OFFSET)
        );

        if (!movR14d) {
            dbg("Unable to find \"mov r14d, [rax+XXXX]\" pattern!");
            return false;
        }

        hde64s hs;
        if (const auto length = hde64_disasm(movR14d, &hs); !length || hs.flags & F_ERROR) {
            dbg("Unable to disasm \"mov r14d, [rax+XXXX]\"");
            return false;
        }

        this->sequenceNumberOffset_ = hs.disp.disp32;
        dbg("g_UserCmd::m_iSequenceNumber offset: %d", this->sequenceNumberOffset_);

        return true;
    }

    datatype::user_cmd_s* C_DotaInput::getUserCmd() const
    {
        const auto localController = this->getScreenPlayerController_(0);
        if (!localController) {
            return nullptr;
        }

        const auto cmdBufferForScreen = this->getScreenCmdBuffer_(this->cmdCircularBuffer_, 0);
        if (!cmdBufferForScreen) {
            return nullptr;
        }

        const auto sequenceNumber = static_cast<uint8_t*>(cmdBufferForScreen)[this->sequenceNumberOffset_];
        if (!sequenceNumber) {
            return nullptr;
        }

        return static_cast<datatype::user_cmd_s*>(this->getUserCmd_(localController, sequenceNumber));
    }

    bool C_DotaInput::initialize()
    {
        const auto clientModule = GetModuleHandleA("client.dll");
        if (!resolveFunctions(clientModule)) {
            dbg("Unable to resolve C_DotaInput functions!");
            return false;
        }

        if (!resolveGlobalCmdBuffer(clientModule)) {
            dbg("Unable to resolve g_UserCmd circular buffer!");
            return false;
        }

        if (!resolveSequenceNumberOffset(clientModule)) {
            dbg("Unable to resolve g_UserCmd::m_iSequenceNumber offset in circular buffer!");
            return false;
        }

        return true;
    }
} // sdk