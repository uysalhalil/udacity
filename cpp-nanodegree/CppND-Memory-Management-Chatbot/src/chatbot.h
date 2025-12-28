#ifndef CHATBOT_H_
#define CHATBOT_H_

#include <wx/bitmap.h>

#include <memory>
#include <string>

using std::string;
using std::unique_ptr;

class GraphNode;  // forward declaration
class ChatLogic;  // forward declaration

class ChatBot {
 private:
  // data handles (owned)
  // wxBitmap* _image;  // avatar image original code using raw pointer
  unique_ptr<wxBitmap> _image;  // avatar image using smart pointer

  // data handles (not owned)
  GraphNode* _currentNode;
  GraphNode* _rootNode;
  ChatLogic* _chatLogic;

  // proprietary functions
  int ComputeLevenshteinDistance(string s1, string s2);

 public:
  // constructors / destructors
  ChatBot();                 // constructor WITHOUT memory allocation
  ChatBot(string filename);  // constructor WITH memory allocation
  ~ChatBot();

  //// STUDENT CODE
  ////

  ChatBot(const ChatBot& other);             // copy constructor
  ChatBot& operator=(const ChatBot& other);  // copy assignment operator
  ChatBot(ChatBot&& other);                  // move constructor
  ChatBot& operator=(ChatBot&& other);       // move assignment operator

  ////
  //// EOF STUDENT CODE

  // getters / setters
  void SetCurrentNode(GraphNode* node);
  void SetRootNode(GraphNode* rootNode) { _rootNode = rootNode; }
  void SetChatLogicHandle(ChatLogic* chatLogic) { _chatLogic = chatLogic; }
  ChatLogic* GetChatLogicHandle() { return _chatLogic; }
  std::unique_ptr<wxBitmap>& GetImageHandle() { return _image; }

  // communication
  void ReceiveMessageFromUser(std::string message);
};

#endif /* CHATBOT_H_ */
