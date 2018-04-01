#include <time.h>
#include <stack>
#include <iostream>
#include <assert.h>

#include "field.h"


struct CELL {
   CELL_TYPE type;
   bool isVisited;
   int position;

   CELL(CELL_TYPE newType, int pos) : type(newType), isVisited(false), position(pos) {}
};


CELL_TYPE FIELD::Get (int i, int j) const
{
   assert(i + j * width < field.size());
   return field[i + j * width];
}

void FIELD::GenerateGameMap(int h, int w)
{
    std::srand(30);

    std::vector <CELL> lab;
    int spaceCount = 0;

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (i % 2 == 0 || j % 2 == 0) {
                lab.push_back(CELL(CELL_TYPE::barrier, i * w + j));
            } else {
                lab.push_back(CELL(CELL_TYPE::space, i * w + j));
                spaceCount++;
            }
        }
    }

    lab[w + 1].isVisited = true;
    int count                = 1;
    int pos                  = w + 1;

    std::stack<CELL> cellStack;
    
    while (count != spaceCount) {
        std::vector<int> freeCellIdx;
        
        if (((pos / w) - ((pos + 2) / w) == 0 && !lab[pos + 2].isVisited)) {
            freeCellIdx.push_back(pos + 2);
        }
        if (((pos / w) - ((pos - 2) / w) == 0 && !lab[pos - 2].isVisited)) {
            freeCellIdx.push_back(pos - 2);
        }
        if ((pos + 2 * w < w * (h - 1) && !lab[pos + 2 * w].isVisited)) {
            freeCellIdx.push_back(pos + 2 * w);
        }
        if ((pos - 2 * w > w && !lab[pos - 2 * w].isVisited)) {
            freeCellIdx.push_back(pos - 2 * w);
        }
        if (freeCellIdx.size() != 0) {
            cellStack.push(lab[pos]);
            int nextPos = freeCellIdx[std::rand() % freeCellIdx.size()];
            
            if (nextPos == pos + 2) {
                lab[pos + 1].type = CELL_TYPE::space;
            } else if (nextPos == pos - 2) {
                lab[pos - 1].type = CELL_TYPE::space;
            } else if (nextPos == pos - 2 * w) {
                lab[pos - w].type = CELL_TYPE::space;
            } else if (nextPos == pos + 2 * w) {
                lab[pos + w].type = CELL_TYPE::space;
            }

            pos = nextPos;
            lab[pos].isVisited = true;
            count++;
        } else if (cellStack.size() != 0) {
            pos = cellStack.top().position;
            cellStack.pop();
        } else {
            //find free cell
            //pos = posOfFreeCell
        }

        freeCellIdx.clear();
    }

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            switch (lab[w * i + j].type) {
            case CELL_TYPE::barrier:
                std::cout << "1 ";
                break;
            case CELL_TYPE::space:
                std::cout << "0 ";
                break;
            }
        }
        std::cout << std::endl;
    }


    for (CELL & i : lab) {
       field.push_back(i.type);
    }
}
