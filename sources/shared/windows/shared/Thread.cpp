#include "stdafx.h"
#include "Thread.h"

namespace shared
{

CThread::CThread()
{
}

CThread::~CThread()
{
}

void CThread::killThread(HANDLE hThread)
{
   TerminateThread(hThread, 0);
}

} // namespace shared