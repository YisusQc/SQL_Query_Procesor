#ifndef FIRST_FOLLOW_H
#define FIRST_FOLLOW_H

#include "Grammar.h"

#include <map>
#include <set>
#include <vector>
#include <string>

using TerminalSet = std::set<TokenType>;

struct SequenceFirst {
  TerminalSet terminals;
  bool derivesEpsilon = false;
};

struct LL1Conflict {
  NonTerminal nonTerminal;
  TokenType lookahead;
  int production1;
  int production2;
};

class FirstFollow {
private:
  const Grammar& grammar;

  std::map<NonTerminal, TerminalSet> firstSets;
  std::map<NonTerminal, TerminalSet> followSets;

  // No terminales que pueden producir epsilon.
  std::set<NonTerminal> nullable;

  void initializeSets();
  void computeFirst();
  void computeFollow();

public:
  explicit FirstFollow(const Grammar& grammar);

  SequenceFirst firstOfSequence(
    const std::vector<Symbol>& sequence
  ) const;

  const TerminalSet& getFirst(NonTerminal nt) const;
  const TerminalSet& getFollow(NonTerminal nt) const;

  bool isNullable(NonTerminal nt) const;

  std::vector<LL1Conflict> findLL1Conflicts() const;
  bool isLL1() const;

  void printFirstSets() const;
  void printFollowSets() const;
  void printLL1Conflicts() const;
};

#endif

