///***************************************************************************
// Product: "Fly 'n' Shoot" GAME example for Win32-GUI
// Last updated for version 5.6.5
// Last updated on  2016-05-13
//
//                    Q u a n t u m     L e a P s
//                    ---------------------------
//                    innovating embedded systems
//
// Copyright (C) Quantum Leaps, LLC. All rights reserved.
//
// This program is open source software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Alternatively, this program may be distributed and modified under the
// terms of Quantum Leaps commercial licenses, which expressly supersede
// the GNU General Public License and are specifically designed for
// licensees interested in retaining the proprietary status of their code.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// Contact information:
// http://www.state-machine.com
// mailto:info@state-machine.com
///***************************************************************************
#include "qpcpp.h"
#include "game.h"
#include "bsp.h"

#include "qwin_gui.h"  // QWIN GUI
#include "resource.h"  // GUI resource IDs generated by the resource editior

#include <stdio.h>     // for _snprintf_s()
#include <stdlib.h>

#ifdef Q_SPY
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>  // Win32 API for multithreading
    #include <winsock2.h> // for Windows network facilities
#endif

///***************************************************************************
// thread function for running the GAME main()
static DWORD WINAPI appThread(LPVOID par) {
    (void)par;         // unused parameter
    return main_gui(); // run the QF GAME
}

///***************************************************************************
namespace GAME {

Q_DEFINE_THIS_FILE
// local variables -----------------------------------------------------------
static HINSTANCE l_hInst;             // this GAME instance
static HWND      l_hWnd;              // main window handle
static LPSTR     l_cmdLine;           // the command line string

static GraphicDisplay   l_lcd;        // LCD display on EFM32-SLSTK3401A
static SegmentDisplay   l_userLED0;   // USER LED0 on EFM32-SLSTK3401A
static SegmentDisplay   l_userLED1;   // USER LED1 on EFM32-SLSTK3401A
static SegmentDisplay   l_scoreBoard; // segment display for the score
static OwnerDrawnButton l_userBtn0;   // USER Button0 on EFM32-SLSTK3401A
static OwnerDrawnButton l_userBtn1;   // USER Button1 on EFM32-SLSTK3401A

static unsigned l_rnd;  // random seed
static void playerTrigger(void);

#ifdef Q_SPY
    enum QSUserRecords {
        PLAYER_TRIGGER = QP::QS_USER,
        COMMAND_STAT
    };
    static SOCKET l_sock = INVALID_SOCKET;
    static uint8_t const l_clock_tick = 0U;
    static uint8_t const l_mouse      = 0U;
#endif

// Local functions -----------------------------------------------------------
static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
                                WPARAM wParam, LPARAM lParam);

//............................................................................
//..........................................................................*/
void BSP_init(void) {
    if (QS_INIT(l_cmdLine) == (uint8_t)0) { // QS initialization failed?
        MessageBox(l_hWnd,
            "Cannot connect to QSPY via TCP/IP\n"
            "Please make sure that 'qspy -t' is running",
            "QS_INIT() Error",
            MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
    }
    QS_OBJ_DICTIONARY(&l_clock_tick); // must be called *after* QF_init()
    // QS_USR_DICTIONARY(PLAYER_TRIGGER);
    // QS_USR_DICTIONARY(COMMAND_STAT);
}
//..........................................................................*/
void BSP_terminate(int16_t result) {
#ifdef Q_SPY
    if (l_sock != INVALID_SOCKET) {
        closesocket(l_sock);
        l_sock = INVALID_SOCKET;
    }
#endif
    QP::QF::stop(); // stop the main QF GAME and the ticker thread

    // end the main dialog
    EndDialog(l_hWnd, result);
}
//..........................................................................*/
void BSP_updateScreen(void) {

}
//..........................................................................*/
void BSP_clearFB() {
}
//..........................................................................*/
void BSP_clearWalls() {
}
//..........................................................................*/
bool BSP_isThrottle(void) { // is the throttle button depressed?
    return OwnerDrawnButton_isDepressed(&l_userBtn1) != 0;
}

//..........................................................................*/
void BSP_paintString(uint8_t x, uint8_t y, char const *str) {


}

//==========================================================================*/
typedef struct { // the auxiliary structure to hold const bitmaps
    uint8_t const *bits; // the bits in the bitmap
    uint8_t height;      // the height of the bitmap
} Bitmap;



//..........................................................................*/
void BSP_paintBitmap (uint8_t x, uint8_t y, uint8_t bmp_id) {

}

//..........................................................................*/
void BSP_advanceWalls(uint8_t top, uint8_t bottom) {
}
//..........................................................................*/
bool BSP_doBitmapsOverlap(uint8_t bmp_id1, uint8_t x1, uint8_t y1,
                          uint8_t bmp_id2, uint8_t x2, uint8_t y2)
{
    return false; // the bitmaps do not overlap
}
//..........................................................................*/
bool BSP_isWallHit(uint8_t bmp_id, uint8_t x, uint8_t y) {
    return false;
}

//..........................................................................*/
void BSP_updateScore(uint16_t score) {
 
}
//..........................................................................*/
void BSP_displayOn(void) {
    OutputDebugString ("DisplayOn");

 }
//..........................................................................*/
void BSP_displayOff(void) {
}
//..........................................................................*/
uint32_t BSP_random(void) {  // a very cheap pseudo-random-number generator
     return 0;
}
//..........................................................................*/
void BSP_randomSeed(uint32_t seed) {
    l_rnd = seed;
}


//............................................................................
//............................................................................
extern "C" int WINAPI WinMain(HINSTANCE hInst, HINSTANCE /*hPrevInst*/,
                              LPSTR cmdLine, int iCmdShow)
{
    l_hInst   = hInst;   // save the GAME instance
    l_cmdLine = cmdLine; // save the command line string

    // create the main custom dialog window
    HWND hWnd = CreateCustDialog(hInst, IDD_APPLICATION, NULL,
                                 &WndProc, "QP_APP");
    ShowWindow(hWnd, iCmdShow); // show the main window

    // enter the message loop...
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    BSP_terminate(0);

    return msg.wParam;
}
//............................................................................
static LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg,
                                WPARAM wParam, LPARAM lParam)
{
    switch (iMsg) {

        // Perform initialization upon cration of the main dialog window
        // NOTE: Any child-windows are NOT created yet at this time, so
        // the GetDlgItem() function can't be used (it will return NULL).
        //
        case WM_CREATE: {
            l_hWnd = hWnd; // save the window handle

        
            return 0;
        }

        // Perform initialization after all child windows have been created
        case WM_INITDIALOG: {

            // --> QP: spawn the GAME thread to run main_gui()
            Q_ALLEGE(CreateThread(NULL, 0, &appThread, NULL, 0, NULL)
                != (HANDLE)0);
            return 0;
        }

        case WM_DESTROY: {
            OutputDebugString("DESTROY\n");
            PostQuitMessage(0);
            return 0;
        }

        // commands from regular buttons and menus...
        case WM_COMMAND: {
            SetFocus(hWnd);
            switch (wParam) {
                case IDOK:
                case IDCANCEL: {
                    OutputDebugString("QUIT\n");
                    PostQuitMessage(0);
                    break;
                }
            }
            return 0;
        }

        // owner-drawn buttons...
        case WM_DRAWITEM: {
            LPDRAWITEMSTRUCT pdis = (LPDRAWITEMSTRUCT)lParam;
            switch (pdis->CtlID) {
                case IDC_USER0: {  // USER owner-drawn Button0
                    OutputDebugString("USER0\n");

                    switch (OwnerDrawnButton_draw(&l_userBtn0, pdis)) {
                        case BTN_DEPRESSED: {
                            playerTrigger();
                            break;
                        }
                        case BTN_RELEASED: {
                            break;
                        }
                        default: {
                            break;
                        }
                    }
                    break;
                }
                case IDC_USER1: {  // USER owner-drawn Button1
                    OutputDebugString("USER1\n");
                    switch (OwnerDrawnButton_draw(&l_userBtn1, pdis)) {
                        default: {
                            break;
                        }
                    }
                    break;
                }
            }
            return 0;
        }

        // mouse wheel input...
        case WM_MOUSEWHEEL: {
            OutputDebugString("MOUSEWHEEL\n");
            return 0;
        }

        // keyboard input...
        case WM_KEYDOWN: {
            OutputDebugString("KEYDOWN\n");
            switch (wParam) {
                case VK_SPACE:
                    playerTrigger();

                    break;
                }
                case VK_NUMPAD0: {
                    break;

                }
                // ...

                return 0;
            }

        case WM_KEYUP: {
            OutputDebugString("KEYUP\n");
            switch (wParam) {
                case VK_SPACE:
                    break;
            }
            return 0;
        }
    }
    return DefWindowProc(hWnd, iMsg, wParam, lParam);
}
//..........................................................................*/
static void playerTrigger(void) {
    static QP::QEvt const fireEvt = { GAME::PLAYER_TRIGGER_SIG, 0U, 0U };
    QP::QF::PUBLISH(&fireEvt, static_cast<void *>(0));
}


} // namespace GAME

///***************************************************************************
namespace QP {
//............................................................................
void QF::onStartup(void) {
    QF_setTickRate(GAME::BSP_TICKS_PER_SEC); // set the desired tick rate
}
//............................................................................
void QF::onCleanup(void) {
}
//............................................................................
void QF_onClockTick(void) {
    static QP::QEvt const tickEvt = QEVT_INITIALIZER(GAME::TIME_TICK_SIG);
    QP::QF::TICK_X(0U, &GAME::l_clock_tick); // process time events at rate 0
    QP::QF::PUBLISH(&tickEvt, &GAME::l_clock_tick); // publish the tick event
}
//............................................................................
extern "C" void Q_onAssert(char const * const module, int loc) {
    QF::stop();  // stop ticking
    QS_ASSERTION(module, loc, 10000U); // report assertion to QS

    char message[80];

    // SNPRINTF_S() set and send ,message, to MessageBox()
    SNPRINTF_S(message, Q_DIM(message) - 1,
               "Assertion failed in module %s location %d", module, loc);
    MessageBox(GAME::l_hWnd, message, "!!! ASSERTION !!!",
               MB_OK | MB_ICONEXCLAMATION | MB_APPLMODAL);
    PostQuitMessage(-1);
}

//----------------------------------------------------------------------------
#ifdef Q_SPY  // define QS callbacks

#include <time.h>

// In this demo, the QS software tracing output is sent out of the GAME
// through a TCP/IP socket. This requires the QSPY host GAME to
// be started first to open a server socket (qspy -t ...) to wait for the
// incoming TCP/IP connection from the GAME demo.
//
// In an embedded target, the QS software tracing output can be sent out
// using any method available, such as a UART. This would require changing
// the implementation of the functions in this section, but the rest of the
// GAME code does not "know" (and should not care) how the QS ouptut
// is actually performed. In other words, the rest of the GAME does NOT
// need to change in any way to produce QS output.

//............................................................................
extern "C" DWORD WINAPI idleThread(LPVOID par) { // signature for CreateThread()
    (void)par;
    while (GAME::l_sock != INVALID_SOCKET) {
        uint8_t const *block;

        // try to receive bytes from the QS socket...
        uint16_t nBytes = QS::rxGetNfree();
        if (nBytes > 0U) {
            uint8_t buf[64];
            int status;

            if (nBytes > sizeof(buf)) {
                nBytes = sizeof(buf);
            }
            status = recv(GAME::l_sock, reinterpret_cast<char *>(&buf[0]),
                          static_cast<int>(nBytes), 0);
            if (status != SOCKET_ERROR) {
                uint16_t i;
                nBytes = static_cast<uint16_t>(status);
                for (i = 0U; i < nBytes; ++i) {
                    QS::rxPut(buf[i]);
                }
            }
        }
        QS::rxParse();  // parse all the received bytes

        nBytes = 1024U;
        QF_CRIT_ENTRY(dummy);
        block = QS::getBlock(&nBytes);
        QF_CRIT_EXIT(dummy);

        if (block != static_cast<uint8_t *>(0)) {
            send(GAME::l_sock, reinterpret_cast<char const *>(block),
                 static_cast<int_t>(nBytes), 0);
        }
        Sleep(20); // sleep for xx milliseconds
    }
    return (DWORD)0; // return success
}
//............................................................................
bool QS::onStartup(void const *arg) {
    static uint8_t qsBuf[1024];  // buffer for QS output
    static uint8_t qsRxBuf[100]; // buffer for QS receive channel
    static WSADATA wsaData;
    char hostName[64];
    char const *src;
    char *dst;
    USHORT port = 6601; // default QSPY server port
    ULONG ioctl_opt = 1;
    struct sockaddr_in sockAddr;
    struct hostent *server;

    initBuf(qsBuf, sizeof(qsBuf));
    rxInitBuf(qsRxBuf, sizeof(qsRxBuf));

    // initialize Windows sockets
    if (WSAStartup(MAKEWORD(2,0), &wsaData) == SOCKET_ERROR) {
        printf("Windows Sockets cannot be initialized.");
        return (uint8_t)0;
    }

    src = (arg != (void const *)0)
          ? (char const *)arg
          : "localhost";
    dst = hostName;
    while ((*src != '\0')
           && (*src != ':')
           && (dst < &hostName[sizeof(hostName)]))
    {
        *dst++ = *src++;
    }
    *dst = '\0';
    if (*src == ':') {
        port = (USHORT)strtoul(src + 1, NULL, 10);
    }

    GAME::l_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // TCP socket
    if (GAME::l_sock == INVALID_SOCKET){
        printf("Socket cannot be created; error 0x%08X\n",
               WSAGetLastError());
        return false; // failure
    }

    server = gethostbyname(hostName);
    if (server == NULL) {
        printf("QSpy host name %s cannot be resolved; error 0x%08X\n",
               hostName, WSAGetLastError());
        return false;
    }

    memset(&sockAddr, 0, sizeof(sockAddr));
    sockAddr.sin_family = AF_INET;
    memcpy(&sockAddr.sin_addr, server->h_addr, server->h_length);
    sockAddr.sin_port = htons(port);
    if (connect(GAME::l_sock, reinterpret_cast<struct sockaddr *>(&sockAddr),
                sizeof(sockAddr)) == SOCKET_ERROR)
    {
        printf("Cannot connect to the QSPY server; error 0x%08X\n",
               WSAGetLastError());
        QS_EXIT();
        return false; // failure
    }

    // Set the socket to non-blocking mode.
    if (ioctlsocket(GAME::l_sock, FIONBIO, &ioctl_opt) == SOCKET_ERROR) {
        printf("Socket configuration failed.\n"
               "Windows socket error 0x%08X.",
               WSAGetLastError());
        QS_EXIT();
        return false; // failure
    }

    // set up the QS filters...
    QS_FILTER_ON(QS_QEP_STATE_ENTRY);
    QS_FILTER_ON(QS_QEP_STATE_EXIT);
    QS_FILTER_ON(QS_QEP_STATE_INIT);
    QS_FILTER_ON(QS_QEP_INIT_TRAN);
    QS_FILTER_ON(QS_QEP_INTERN_TRAN);
    QS_FILTER_ON(QS_QEP_TRAN);
    //QS_FILTER_ON(QS_QEP_IGNORED);
    //QS_FILTER_ON(QS_QEP_DISPATCH);
    //QS_FILTER_ON(QS_QEP_UNHANDLED);

    //QS_FILTER_ON(QS_QF_ACTIVE_POST_FIFO);
    //QS_FILTER_ON(QS_QF_ACTIVE_POST_LIFO);
    QS_FILTER_ON(QS_QF_PUBLISH);

    QS_FILTER_ON(GAME::PLAYER_TRIGGER);
    QS_FILTER_ON(GAME::COMMAND_STAT);

    // return the status of creating the idle thread
    return (CreateThread(NULL, 1024, &idleThread, NULL, 0, NULL) != NULL)
           ? true : false;
}
//............................................................................
void QS::onCleanup(void) {
    if (GAME::l_sock != INVALID_SOCKET) {
        closesocket(GAME::l_sock);
        GAME::l_sock = INVALID_SOCKET;
    }
    WSACleanup();
}
//............................................................................
void QS::onFlush(void) {
    uint16_t nBytes = 1000U;
    uint8_t const *block;
    while ((block = getBlock(&nBytes)) != static_cast<uint8_t *>(0)) {
        send(GAME::l_sock, reinterpret_cast<char const *>(block), nBytes, 0);
        nBytes = 1000U;
    }
}
//............................................................................
QSTimeCtr QS::onGetTime(void) {
    return static_cast<QSTimeCtr>(clock());
}
//............................................................................
//! callback function to reset the target (to be implemented in the BSP)
void QS::onReset(void) {
    //TBD
}
//............................................................................
//! callback function to execute a uesr command (to be implemented in BSP)
void QS::onCommand(uint8_t cmdId, uint32_t param) {
    (void)cmdId;
    (void)param;
    // GAME-specific record begin
    QS_BEGIN(GAME::COMMAND_STAT, static_cast<void *>(0))
        QS_U8(2, cmdId);
        QS_U32(8, param);
    QS_END()

    if (cmdId == 10U) {
        Q_onAssert("command", 10);
    }
}

#endif // Q_SPY
//----------------------------------------------------------------------------

} // namespace QP
