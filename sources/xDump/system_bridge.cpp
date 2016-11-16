#include "config_parser.h"
#include "system_bridge.h"

namespace xDump {

void SystemBridge::transferToParser(QString file, QString section){
    parser.parseConfig(file, section);
}

}
