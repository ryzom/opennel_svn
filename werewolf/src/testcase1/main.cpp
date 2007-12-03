#include "ww/wwcommon/IBaseSimulation.h"

namespace WWTESTCASE1
{

class CTestSimulation : public WWCOMMON::IBaseSimulation
{


}; // END CLASS CTESTSIMULATION

WWCOMMON::IBaseSimulation *Simulation;

WWCOMMON::IBaseSimulation *getSimulation() {
        if(Simulation==NULL) {
                Simulation=static_cast<WWCOMMON::IBaseSimulation *>(new CTestSimulation());
        }
        return static_cast<CClientSimulation *>(Simulation);
}
CTestSimulation *getClientSimulation() {
        if(Simulation==NULL) {
                Simulation=static_cast<WWCOMMON::IBaseSimulation *>(new CTestSimulation());
        }
        return static_cast<CTestSimulation *>(Simulation);
}




{; // END OF NAMESPACE WWTESTCASE1

int main() {
	return 0;
}
