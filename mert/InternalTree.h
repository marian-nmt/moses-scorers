#pragma once

#include <memory>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "util/exception.hh"
#include "util/generator.hh"

namespace MosesTuning {

class InternalTree;
typedef std::shared_ptr<InternalTree> TreePointer;
typedef int NTLabel;

class InternalTree {
  std::string m_value;
  std::vector<TreePointer> m_children;
  bool m_isTerminal;

public:
  InternalTree(const std::string& line, const bool terminal = false);
  InternalTree(const InternalTree& tree) : m_value(tree.m_value), m_isTerminal(tree.m_isTerminal) {
    const std::vector<TreePointer>& children = tree.m_children;
    for(std::vector<TreePointer>::const_iterator it = children.begin(); it != children.end(); it++) {
      m_children.emplace_back(new InternalTree(**it));
    }
  }
  size_t AddSubTree(const std::string& line, size_t start);

  std::string GetString(bool start = true) const;
  void Combine(const std::vector<TreePointer>& previous);
  const std::string& GetLabel() const { return m_value; }

  size_t GetLength() const { return m_children.size(); }
  std::vector<TreePointer>& GetChildren() { return m_children; }

  bool IsTerminal() const { return m_isTerminal; }

  bool IsLeafNT() const { return (!m_isTerminal && m_children.size() == 0); }
};

// Python-like generator that yields next nonterminal leaf on every call
$generator(leafNT) {
  std::vector<TreePointer>::iterator it;
  InternalTree* tree;
  leafNT(InternalTree* root = 0) : tree(root) {}
  $emit(std::vector<TreePointer>::iterator) for(it = tree->GetChildren().begin();
                                                it != tree->GetChildren().end();
                                                ++it) {
    if(!(*it)->IsTerminal() && (*it)->GetLength() == 0) {
      $yield(it);
    } else if((*it)->GetLength() > 0) {
      if((*it).get()) {  // normal pointer to same object that TreePointer points to
        $restart(tree = (*it).get());
      }
    }
  }
  $stop;
};

}  // namespace MosesTuning