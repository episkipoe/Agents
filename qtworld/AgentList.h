#ifndef AGENT_LIST
#include <QComboBox>

class AgentList: public QComboBox {
public:
	AgentList(QWidget * p); 
private:
};

#include <QPushButton>
class AgentListButton : public QPushButton {
public:
	AgentListButton(const QString & txt, QWidget * p, AgentList * a) : QPushButton(txt, p), agent_list(a) { }
	AgentList * get_agent_list(void) { return agent_list; }
private:
	AgentList * agent_list;
		
};


#endif 
