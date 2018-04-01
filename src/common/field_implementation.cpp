#include "field_implementation.h"

void FIELD_IMPLEMENTATION::GenerateGameMap(int height, int width)
{
    std::srand(time(0));

    std::vector <CELL> lab;
    int spaceCount = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i % 2 == 0 || j % 2 == 0) {
                lab.push_back(CELL(CELL_TYPE::barrier, i * width + j));
            } else {
                lab.push_back(CELL(CELL_TYPE::space, i * width + j));
                spaceCount++;
            }
        }
    }

    lab[width + 1].isVisited = true;
    int count                = 1;
    int pos                  = width + 1;

    std::stack<CELL> cellStack;
    
    while (count != spaceCount) {
        std::vector<int> freeCellIdx;
        
        if (((pos / width) - ((pos + 2) / width) == 0 && !lab[pos + 2].isVisited)) {
            freeCellIdx.push_back(pos + 2);
        }
        if (((pos / width) - ((pos - 2) / width) == 0 && !lab[pos - 2].isVisited)) {
            freeCellIdx.push_back(pos - 2);
        }
        if ((pos + 2 * width < width * (height - 1) && !lab[pos + 2 * width].isVisited)) {
            freeCellIdx.push_back(pos + 2 * width);
        }
        if ((pos - 2 * width > width && !lab[pos - 2 * width].isVisited)) {
            freeCellIdx.push_back(pos - 2 * width);
        }
        if (freeCellIdx.size() != 0) {
            cellStack.push(lab[pos]);
            int nextPos = freeCellIdx[std::rand() % freeCellIdx.size()];
            
            if (nextPos == pos + 2) {
                lab[pos + 1].type = CELL_TYPE::space;
            } else if (nextPos == pos - 2) {
                lab[pos - 1].type = CELL_TYPE::space;
            } else if (nextPos == pos - 2 * width) {
                lab[pos - width].type = CELL_TYPE::space;
            } else if (nextPos == pos + 2 * width) {
                lab[pos + width].type = CELL_TYPE::space;
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

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            switch (lab[width * i + j].type) {
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
