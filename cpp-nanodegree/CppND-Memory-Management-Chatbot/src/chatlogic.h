#ifndef CHATLOGIC_H_
#define CHATLOGIC_H_

#include <memory>
#include <string>
#include <vector>

#include "chatgui.h"

using std::pair;
using std::string;
using std::unique_ptr;
using std::vector;

// forward declarations
class ChatBot;
class GraphEdge;
class GraphNode;

class ChatLogic {
 private:
  //// STUDENT CODE
  ////

  // data handles (owned)
  //   std::vector<GraphNode*> _nodes;
  std::vector<std::unique_ptr<GraphNode>> _nodes;
  //   std::vector<std::unique_ptr<GraphEdge>> _edges;
  //   std::vector<GraphEdge*> _edges;

  ////
  //// EOF STUDENT CODE

  // data handles (not owned)
  unique_ptr<ChatBot> _chatBot;
  unique_ptr<GraphNode> _currentNode;
  ChatBotPanelDialog* _panelDialog;

  // proprietary type definitions
  typedef vector<pair<string, string>> tokenlist;

  // proprietary functions
  template <typename T>
  void AddAllTokensToElement(string tokenID, tokenlist& tokens, T& element);

 public:
  // constructor / destructor
  ChatLogic();
  ~ChatLogic();

  // getter / setter
  void SetPanelDialogHandle(ChatBotPanelDialog* panelDialog);
  void SetChatbotHandle(ChatBot* chatbot);

  // proprietary functions
  void LoadAnswerGraphFromFile(string filename);
  void SendMessageToChatbot(string message);
  void SendMessageToUser(string message);
  wxBitmap* GetImageFromChatbot();
};

#endif /* CHATLOGIC_H_ */