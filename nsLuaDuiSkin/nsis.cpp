#include "nsis.h"


NsisMessageFilter Nsis::n_filter;
extra_parameters* Nsis::m_extra;
INT_PTR Nsis::m_installfucntion;
int Nsis::m_progressfunction;
std::map<const char*, int> Nsis::m_functions;