#ifndef MSGDEF_H
#define MSGDEF_H
#pragma once

#include <WinUser.h>

#define WM_NSIS_MIN								WM_USER + 888
#define WM_NSIS_CLOSE							WM_USER + 889
#define WM_NSIS_BACK							WM_USER + 890
#define WM_NSIS_NEXT							WM_USER + 891
#define WM_NSIS_CANCEL							WM_USER + 892
#define WM_NSIS_INSTALL							WM_USER + 893
#define WM_NSIS_STATE							WM_USER + 894
#define WM_NSIS_FINISHEDINSTALL					WM_USER + 895
#define WM_NSIS_OPENURL                         WM_USER + 896
#define WM_NSIS_UNINSTALL						WM_USER + 897

#endif