#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SCREEN_WIDTH 830
#define SCREEN_HEIGHT 550

#define TARGET_FPS 8000

#define BACKGROUND_COLOR (Color) {210, 210, 210, 255}
#define POINT_COLOR (Color) {105, 88, 21, 255}
#define ROAD_COLOR (Color) {20, 20, 20, 255}

#define INNER_WINDOW_WIDTH 550 
#define INNER_WINDOW_HEIGHT 300

#define GLOBAL_HEADER_FONT_SIZE 25
#define GLOBAL_MAIN_FONT_SIZE 18

#define POINT_SIZE 5
#define ROAD_THICKNESS 3

#define ARRAY_SIZE(arr) (sizeof(arr)/sizeof(arr[0])) // Macros are cool

typedef struct {
    int posX, posY;
} Point;

struct Node {
    unsigned int valueIndex;
    struct Node* childNodes[10]; // Max number of child nodes
};

typedef struct {
    struct Node* rootNode;
} Tree;

Point* GeneratePointMap(const unsigned int* numberOfPoints){
    Point* pointMap = malloc(sizeof(Point) * (*numberOfPoints)); 

    for(int i = 0; i < (*numberOfPoints); i++){
        Vector2 newPosition;
        newPosition.x =
            GetRandomValue(((float)SCREEN_WIDTH/2) - ((float)INNER_WINDOW_WIDTH/2) + POINT_SIZE + 2,
                           ((float)SCREEN_WIDTH/2) + ((float)INNER_WINDOW_WIDTH/2) - POINT_SIZE - 2);
        newPosition.y =
            GetRandomValue(((float)SCREEN_HEIGHT/2) - ((float)INNER_WINDOW_HEIGHT/2) + POINT_SIZE + 2,
                           ((float)SCREEN_HEIGHT/2) + ((float)INNER_WINDOW_HEIGHT/2) - POINT_SIZE - 2);

        pointMap[i].posX = newPosition.x;
        pointMap[i].posY = newPosition.y;
    }
    return pointMap;
}

struct Node* CreateNewNode(unsigned int indexValue){
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->valueIndex = indexValue;
    return newNode;
}

void PopulateIndexDataArray(int baseData[], int* newData, int ignoreValue, int arraySize){
    // Length of newData must be length of baseData-1
    // arraySize is the size of the baseData!!!
    int s = 0;
    for(int i = 0; i < arraySize; i++){
        if(baseData[i] == ignoreValue) {continue;}
        newData[s] = baseData[i];
        s++;
    }
}

void PopulateTreeLayer(struct Node* parentNode, int parentLayerData[], int parentIndexDataSize) {
        if (parentIndexDataSize <= 1)
            return;
        // Generating Layer of node parentNode
        int indexData[parentIndexDataSize-1];
        PopulateIndexDataArray(parentLayerData, indexData,
                               parentNode->valueIndex, parentIndexDataSize);

        for(int n = 0; n < parentIndexDataSize-1; n++){
            struct Node* newChildNode = CreateNewNode(indexData[n]);
            parentNode->childNodes[n] = newChildNode;

            PopulateTreeLayer(newChildNode, indexData, parentIndexDataSize-1);
        }
}

void StepTreeMatrixArray(int* refArray, int boundsArray[], int numberOfPoints, unsigned int* solutionsTried){
     (*solutionsTried)++;
     for(int i = 0; i < numberOfPoints; i++){
         bool carry = false;
         refArray[i]++;
         if(refArray[i] > boundsArray[i]){
             carry = true;
             refArray[i] = 0;
         }

         if(!carry) return;
     }
}

void GenerateNewPermutationData(int* permutaionData, Tree* usingTree, int matrixPath[], int numberOfPoints){
    struct Node* currentNode = usingTree->rootNode;
    permutaionData[0] = currentNode->valueIndex;

    for(int i = 1; i < numberOfPoints; i++){
        currentNode = currentNode->childNodes[matrixPath[i]];
        permutaionData[i] = currentNode->valueIndex;
    }
}

int Factorial(const unsigned int* n){
    if(*n == 0) return 1;
    int t = 1;
    for(int i = *n; i > 0; i--){
        t *= i;
    }
    return t;
}

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_WINDOW_HIGHDPI); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "C Travelling Salesman Visualizer");

    SetTargetFPS(TARGET_FPS);

    const unsigned int numberOfPoints = 10; // Anything above 10 will make the program terminate lmao
    const bool hyperOptimizationMode = false;
    const bool showOnlyBest = true;


    // Main Data
    
    unsigned int solutionsTried = 0;
    unsigned int maxSolutions = Factorial(&numberOfPoints);

    float bestDistance = SCREEN_HEIGHT * SCREEN_WIDTH;
    bool updateDistanceCounter = true;
    int bestPermutaionPath[numberOfPoints];

    bool simulationFinished = false;
    int elapsedTime;

    //--------------------------------------------------------------------------------------
   
    printf("\nGenerating Permutation Data...\n");

    Point* pointData = GeneratePointMap(&numberOfPoints);

    
    int permutaionData[numberOfPoints];
    
    for(int k = 0; k < numberOfPoints; k++){
        permutaionData[k] = k;
    }
    
    Tree* permutationTrees[numberOfPoints];
    // Generating permutation trees
    
    for(int t = 0; t < numberOfPoints; t++){
        permutationTrees[t] = malloc(sizeof(Tree));
    }

    for(int r = 0; r < numberOfPoints; r++){ // Top level loop for each tree of root node type r
        struct Node* rootNode = CreateNewNode(r);

        PopulateTreeLayer(rootNode, permutaionData, numberOfPoints); 
        permutationTrees[r]->rootNode = rootNode;
    }

    // Tree reference index arrys

    int maxIndex[numberOfPoints];
    int in = 0;
    for(int m = numberOfPoints-1; m >= 0; m--){
        maxIndex[in] = m;
        in++;
    }

    int treeMatrixReferenceArray[numberOfPoints];
    for(int tm = 0; tm < numberOfPoints; tm++) treeMatrixReferenceArray[tm] = 0;

    printf("Generation OK!\nStarting Traversal...\n");

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update --------------------------------------------------------------------------

        if(solutionsTried < maxSolutions){

            GenerateNewPermutationData(permutaionData,
                                       permutationTrees[permutaionData[0]],
                                       treeMatrixReferenceArray, numberOfPoints);
            StepTreeMatrixArray(treeMatrixReferenceArray, maxIndex, numberOfPoints, &solutionsTried);

        }
        else if(solutionsTried == maxSolutions && !simulationFinished){
            for(int g = 0; g < numberOfPoints; g++){permutaionData[g] = bestPermutaionPath[g];}
            simulationFinished = true;
        }
        if(!simulationFinished) {elapsedTime = (int)GetTime();}
        // Draw ----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);
        for(int p = 0; p < numberOfPoints; p++){ // Drawing points
            DrawCircle(pointData[p].posX, pointData[p].posY, POINT_SIZE, POINT_COLOR);
        }

        Vector2 pointPosition1;
        Vector2 pointPosition2;

        float totalDistance = 0;

        for(int l = 0; l < numberOfPoints - 1; l++){
            pointPosition1.x = pointData[permutaionData[l]].posX;
            pointPosition1.y = pointData[permutaionData[l]].posY;

            pointPosition2.x = pointData[permutaionData[l+1]].posX;
            pointPosition2.y = pointData[permutaionData[l+1]].posY;

            if(!simulationFinished){
                float dx = abs((int)pointPosition1.x - (int)pointPosition2.x);
                float dy = abs((int)pointPosition1.y - (int)pointPosition2.y);

                float distance = sqrt(pow(dx, 2) + pow(dy, 2));
                totalDistance += distance;
            }

            if(!showOnlyBest)DrawLineEx(pointPosition1, pointPosition2, ROAD_THICKNESS, ROAD_COLOR);
        }


        if(!simulationFinished && totalDistance < bestDistance) {
            bestDistance = totalDistance;
            updateDistanceCounter = true;
            for(int c = 0; c < numberOfPoints; c++) // For you my friend :)
            {bestPermutaionPath[c] = permutaionData[c];}

        }
        else
            updateDistanceCounter = false;
         
        if(showOnlyBest){
            for(int b = 0; b < numberOfPoints-1; b++){
                pointPosition1.x = pointData[bestPermutaionPath[b]].posX;
                pointPosition1.y = pointData[bestPermutaionPath[b]].posY;

                pointPosition2.x = pointData[bestPermutaionPath[b+1]].posX;
                pointPosition2.y = pointData[bestPermutaionPath[b+1]].posY;
                
                DrawLineEx(pointPosition1, pointPosition2, ROAD_THICKNESS, ROAD_COLOR);
            }
        }


        if(!showOnlyBest){
            // Connecting for full loop
            pointPosition1.x =
                pointData[permutaionData[numberOfPoints-1]].posX;    
            pointPosition1.y =
                pointData[permutaionData[numberOfPoints-1]].posY;    


            pointPosition2.x = pointData[permutaionData[0]].posX;
            pointPosition2.y = pointData[permutaionData[0]].posY;
        
            DrawLineEx(pointPosition1, pointPosition2, ROAD_THICKNESS, ROAD_COLOR);
        }
        else{
            pointPosition1.x =
                pointData[bestPermutaionPath[numberOfPoints-1]].posX;    
            pointPosition1.y =
                pointData[bestPermutaionPath[numberOfPoints-1]].posY;    


            pointPosition2.x = pointData[bestPermutaionPath[0]].posX;
            pointPosition2.y = pointData[bestPermutaionPath[0]].posY;
        
            DrawLineEx(pointPosition1, pointPosition2, ROAD_THICKNESS, ROAD_COLOR);
        }

        // Text UI setup
        
        if(!hyperOptimizationMode){
            char charDataBuffer[100];
            sprintf(charDataBuffer, "Time Elapsed : %d", elapsedTime);
            sprintf(charDataBuffer + strlen(charDataBuffer), "\nSolutions Tried : %d/%d", solutionsTried, maxSolutions);

            sprintf(charDataBuffer + strlen(charDataBuffer), "\nBest Distance: %f", bestDistance);

            DrawText("C Travelling Salesman Visualizer", 5, 5, GLOBAL_HEADER_FONT_SIZE , GRAY);
            DrawText(charDataBuffer, 20, 40, GLOBAL_MAIN_FONT_SIZE , GRAY);

            // Inner window setup
        
            struct Rectangle innerWindowRect;
            innerWindowRect.x = ((float)SCREEN_WIDTH/2) - ((float)INNER_WINDOW_WIDTH/2);
            innerWindowRect.y = ((float)SCREEN_HEIGHT/2) - ((float)INNER_WINDOW_HEIGHT/2);
            innerWindowRect.width = INNER_WINDOW_WIDTH;
            innerWindowRect.height = INNER_WINDOW_HEIGHT;

            struct Rectangle* _innerWindowRect = &innerWindowRect;

            DrawRectangleLinesEx(*_innerWindowRect, 5.0f, BLACK);
        }
        
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    free(pointData);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
