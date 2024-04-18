#ifndef TREENODE_H
#define TREENODE_H 

#include "game.h"
#include "cpuplayer.h"
#include <memory>
#include <vector>

#define SIM_COUNT 1
#define EXPLOIT_EXPLORE 2.8

class MctsNode;

class MctsCpuPlayer : public MinMaxCpuPlayer {
public:
  MctsCpuPlayer(char p,
                 float (*aval)(Game& game, const MinMaxCpuPlayer *const self),
                 int max_depth,
                 int idx);
  MctsCpuPlayer(const MctsCpuPlayer& other);

  std::unique_ptr<MctsNode> tree_root;
  ~MctsCpuPlayer() override = default;

  Coord choseSquare(Game& game) override;

private:
  void step(Coord last_move);
};

class MctsNode {
public:
    MctsNode(Coord move, int played_by, int wins);
    MctsNode(const MctsNode& other);

    int wins;
    float uct;
    int visit_count;
    Coord move;
    int played_by;
    bool fuly_expanded;
    bool terminal;
    MctsNode * parent;
    std::vector<std::unique_ptr<MctsNode>> children;

    void select(std::vector<MctsNode*>& fill);
    void expand_simulate_backpropagte(Game& game, MctsCpuPlayer * self);
private:
    void backpropagate(const Game * const game, MctsCpuPlayer * self, int win_count);
    void self_aval(const Game * const game, const MctsCpuPlayer * self, int win_count);
    void add_child(Coord move, int played_by, int win_count);
};



#endif
