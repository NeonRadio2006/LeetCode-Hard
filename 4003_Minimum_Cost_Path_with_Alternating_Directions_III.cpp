// It is a good Dijkstra Question(Was not able to solve during the contest)
class Solution {
public:
    // Implementing using struct to make the code more readable
    struct State{
        long long cost;
        int row;
        int col;
        int parity;
    };
    // Custom Comparator is needed for priority queue as we have defined custom data type for priority queue
    // In heaps we write comparator in opposite fashion
    // Comparator for min heap
    struct Comp{
        bool operator()(const State& a,const State& b)const{
            return a.cost>b.cost;
        }
    };
    long long minCost(int m, int n, vector<vector<int>>& penalty) {
        // dist[r][c][parity] will store the cost require to reach this particular state
        vector<vector<vector<long long>>>dist(m,vector<vector<long long>>(n,vector<long long>(2,4e18)));
        // Min-Heap with custom state and comparator
        priority_queue<State,vector<State>,Comp>pq;
        // Initializing the dist 3D vector and pushing the first cell into the priority queue
        dist[0][0][1]=1;
        pq.push({1,0,0,1});
        // Arrays for moving to any adjacent cell
        int dr[4]={-1,1,0,0};
        int dc[4]={0,0,-1,1};
        while(!pq.empty()){
            State currState=pq.top();
            pq.pop();
            // Extracting current cost,row,column and parity
            long long currCost=currState.cost;
            int currRow=currState.row;
            int currCol=currState.col;
            int currParity=currState.parity;
            // Stale State check
            if(currCost!=dist[currRow][currCol][currParity]){
                continue;
            }
            // Return the cost if we have reached the destination
            if(currRow==m-1&&currCol==n-1){
                return currCost;
            }
            // We have total of 2 options when we are at any cell
            // Option 1:-To Wait,If we wait we have to consider the penalty and the parity also changes
            long long costIfWeWait=currCost+penalty[currRow][currCol];
            // If the cost incurred after waiting is lesser than greedily assign it 
            if(costIfWeWait<dist[currRow][currCol][currParity^1]){
                dist[currRow][currCol][currParity^1]=costIfWeWait;
                pq.push({costIfWeWait,currRow,currCol,currParity^1});
            }
            // Option 2:-To Move to any valid adjacent cell
            for(int k=0;k<4;k++){
                int nr=currRow+dr[k]; // New Row
                int nc=currCol+dc[k]; // New Column
                // Check if the new Row and Column are valid or not
                if(nr>=0&&nr<m&&nc>=0&&nc<n){
                    // We have to pay the cost of entering the cell no matter if the parity rule is followed or not
                    long long entryCost=1LL*(nr+1)*(nc+1);
                    bool followsParity=false;
                    // Checking if the parity rule is followed or not
                    if(currParity==1){
                        if((dr[k]==0&&dc[k]==1)||(dr[k]==1&&dc[k]==0)){
                            followsParity=true;
                        }
                    }
                    else{
                        if((dr[k]==0&&dc[k]==-1)||(dr[k]==-1&&dc[k]==0)){
                            followsParity=true;
                        }
                    }
                    // If the parity rule is not followed then add the penalty
                    if(!followsParity){
                        entryCost+=penalty[currRow][currCol];
                    }
                    // Final Cost of moving to a new cell
                    long long newCost=currCost+entryCost;
                    // Greedily assigning if the Final Cost is lesser than the previous cost and also pushing it into the priority queue
                    if(newCost<dist[nr][nc][currParity^1]){
                        dist[nr][nc][currParity^1]=newCost;
                        pq.push({newCost,nr,nc,currParity^1});
                    }
                }
            }
        }
        // This statement never executes
        return -1;
    }
};
