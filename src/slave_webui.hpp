#ifndef SLAVE_WEBUI_HPP
#define SLAVE_WEBUI_HPP

#include <process.hpp>

#include "config.hpp"
#include "slave.hpp"

#ifdef NEXUS_WEBUI

namespace nexus { namespace internal { namespace slave {

void startSlaveWebUI(PID slave);

}}} /* namespace */

#endif /* NEXUS_WEBUI */

#endif /* SLAVE_WEBUI_HPP */