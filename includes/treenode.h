#ifndef TREENODE_H
#define TREENODE_H 

#include "game.h"
#include "cpuplayer.h"
#include <memory>
#include <vector>
class MctsNode {
public:
    MctsNode(Coord move, int played_by, float playout_val);
    float value;
    float uct;
    int visit_count;
    Coord move;
    int played_by;
    MctsNode * parent;
    std::vector<std::unique_ptr<MctsNode>> children;

    void select(std::vector<MctsNode*>& fill);
    void expand_simulate(Game& game, MctsCpuPlayer * self);
    void backpropagate();
private:
    void self_aval();
};

#endif
