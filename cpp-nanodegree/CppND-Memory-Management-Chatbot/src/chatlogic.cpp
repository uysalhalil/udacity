#include "chatlogic.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <tuple>
#include <vector>

#include "chatbot.h"
#include "graphedge.h"
#include "graphnode.h"

using std::find_if;
using std::make_unique;
using std::move;
using std::pair;
using std::stoi;
using std::string;
using std::unique_ptr;
using std::vector;

ChatLogic::ChatLogic() {
  //// STUDENT CODE
  ////

  // create instance of chatbot
  _chatBot = make_unique<ChatBot>("../images/chatbot.png");

  // add pointer to chatlogic so that chatbot answers can be passed on to the
  // GUI
  _chatBot->SetChatLogicHandle(this);

  ////
  //// EOF STUDENT CODE
}

ChatLogic::~ChatLogic() {
  //// STUDENT CODE
  ////

  // delete chatbot instance
  // no need to delete _chatBot since it is managed by unique_ptr

  // delete all nodes
  //   for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it) {
  //     delete *it;
  //   }

  // delete all edges
  //   for (auto it = std::begin(_edges); it != std::end(_edges); ++it) {
  //     delete *it;
  //   }

  ////
  //// EOF STUDENT CODE
}

template <typename T>
void ChatLogic::AddAllTokensToElement(string tokenID, tokenlist& tokens, T& element) {
  // find all occurences for current node
  auto token = tokens.begin();
  while (true) {
    token =
        find_if(token, tokens.end(), [&tokenID](const pair<string, string>& pair) { return pair.first == tokenID; });
    if (token != tokens.end()) {
      element.AddToken(token->second);  // add new keyword to edge
      token++;                          // increment iterator to next element
    } else {
      break;  // quit infinite while-loop
    }
  }
}

void ChatLogic::LoadAnswerGraphFromFile(string filename) {
  // load file with answer graph elements
  std::ifstream file(filename);

  // check for file availability and process it line by line
  if (file) {
    // loop over all lines in the file
    string lineStr;
    while (getline(file, lineStr)) {
      // extract all tokens from current line
      tokenlist tokens;
      while (lineStr.size() > 0) {
        // extract next token
        auto posTokenFront = lineStr.find("<");
        auto posTokenBack = lineStr.find(">");
        if (posTokenFront < 0 || posTokenBack < 0) break;  // quit loop if no complete token has been found
        string tokenStr = lineStr.substr(posTokenFront + 1, posTokenBack - posTokenFront - 1);

        // extract token type and info
        auto posTokenInfo = tokenStr.find(":");
        if (posTokenInfo != string::npos) {
          string tokenType = tokenStr.substr(0, posTokenInfo);
          string tokenInfo = tokenStr.substr(posTokenInfo + 1);

          // add token to vector
          tokens.push_back(make_pair(tokenType, tokenInfo));
        }

        // remove token from current line
        lineStr = lineStr.substr(posTokenBack + 1, lineStr.size());
      }

      // process tokens for current line
      auto type =
          find_if(tokens.begin(), tokens.end(), [](const pair<string, string>& pair) { return pair.first == "TYPE"; });
      if (type != tokens.end()) {
        // check for id
        auto idToken =
            find_if(tokens.begin(), tokens.end(), [](const pair<string, string>& pair) { return pair.first == "ID"; });
        if (idToken != tokens.end()) {
          // extract id from token
          int id = stoi(idToken->second);

          // node-based processing
          if (type->second == "NODE") {
            //// STUDENT CODE
            ////

            // check if node with this ID exists already
            auto newNode = find_if(_nodes.begin(), _nodes.end(),
                                   [&id](unique_ptr<GraphNode>& node) { return node->GetID() == id; });

            // create new element if ID does not yet exist
            if (newNode == _nodes.end()) {
              _nodes.emplace_back(make_unique<GraphNode>(id));
              newNode = _nodes.end() - 1;  // get iterator to last element

              // add all answers to current node
              AddAllTokensToElement("ANSWER", tokens, **newNode);
            }

            ////
            //// EOF STUDENT CODE
          }

          // edge-based processing
          if (type->second == "EDGE") {
            //// STUDENT CODE
            ////

            // find tokens for incoming (parent) and outgoing (child) node
            auto parentToken = find_if(tokens.begin(), tokens.end(),
                                       [](const pair<string, string>& pair) { return pair.first == "PARENT"; });
            auto childToken = find_if(tokens.begin(), tokens.end(),
                                      [](const pair<string, string>& pair) { return pair.first == "CHILD"; });

            if (parentToken != tokens.end() && childToken != tokens.end()) {
              // get iterator on incoming and outgoing node via ID search
              auto parentNode = find_if(_nodes.begin(), _nodes.end(), [&parentToken](unique_ptr<GraphNode>& node) {
                return node->GetID() == stoi(parentToken->second);
              });
              auto childNode = find_if(_nodes.begin(), _nodes.end(), [&childToken](unique_ptr<GraphNode>& node) {
                return node->GetID() == stoi(childToken->second);
              });

              // create new edge
              unique_ptr<GraphEdge> edge = make_unique<GraphEdge>(id);
              edge->SetChildNode(childNode->get());
              edge->SetParentNode(parentNode->get());
              _edges.push_back(std::move(edge));

              // find all keywords for current node
              AddAllTokensToElement("KEYWORD", tokens, *edge);

              // store reference in child node and parent node
              (*childNode)->AddEdgeToParentNode(move(edge));
              (*parentNode)->AddEdgeToChildNode(move(edge));
            }

            ////
            //// EOF STUDENT CODE
          }
        } else {
          std::cout << "Error: ID missing. Line is ignored!" << std::endl;
        }
      }
    }  // eof loop over all lines in the file

    file.close();

  }  // eof check for file availability
  else {
    std::cout << "File could not be opened!" << std::endl;
    return;
  }

  //// STUDENT CODE
  ////

  // identify root node
  GraphNode* rootNode = nullptr;
  for (auto it = std::begin(_nodes); it != std::end(_nodes); ++it) {
    // search for nodes which have no incoming edges
    if ((*it)->GetNumberOfParents() == 0) {
      if (rootNode == nullptr) {
        rootNode = it->get();  // assign current node to root
      } else {
        std::cout << "ERROR : Multiple root nodes detected" << std::endl;
      }
    }
  }

  // add chatbot to graph root node
  _chatBot->SetRootNode(rootNode);
  rootNode->MoveChatbotHere(move(_chatBot));

  ////
  //// EOF STUDENT CODE
}

void ChatLogic::SetPanelDialogHandle(ChatBotPanelDialog* panelDialog) { _panelDialog = panelDialog; }

void ChatLogic::SetChatbotHandle(ChatBot* chatbot) { _chatBot = make_unique<ChatBot>(*chatbot); }

void ChatLogic::SendMessageToChatbot(std::string message) { _chatBot->ReceiveMessageFromUser(message); }

void ChatLogic::SendMessageToUser(std::string message) { _panelDialog->PrintChatbotResponse(message); }

wxBitmap* ChatLogic::GetImageFromChatbot() { return _chatBot->GetImageHandle().get(); }
