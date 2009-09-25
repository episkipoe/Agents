#include <vector>
#include "agent.h"
#include "AgentList.h"

using namespace std;
extern vector <Agent *> agents;

AgentList::AgentList(QWidget * p): QComboBox(p) {
	for (agent_iter iter = agents.begin() ; iter != agents.end() ; iter++) {
		addItem(tr((*iter)->getName().c_str()));
	}
}
