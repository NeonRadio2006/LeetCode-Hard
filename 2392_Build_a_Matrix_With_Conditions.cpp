class Solution {
public:
    //Kahn's Algo + Cycle detection(for impossible case)
    vector<int>kahnTopo(int k,vector<vector<int>>&conditions){
        vector<vector<int>>adj(k+1);
        vector<int>inDegree(k+1,0);
        vector<int>ans;
        // Building adj for a particular conditions vector
        for(auto& condition:conditions){
            int u=condition[0];
            int v=condition[1];
            adj[u].push_back(v);
            //incrementing the indegree for node v as the edge is u->v
            inDegree[v]++;
        }
        queue<int>q;
        // Pushing the nodes which are independent
        for(int i=1;i<=k;i++){
            if(inDegree[i]==0){
                q.push(i);
            }
        }
        while(!q.empty()){
            int node=q.front();
            q.pop();
            // Adding to the answer
            ans.push_back(node);
            // Decrementing the in degree for all the neighbour nodes in which there is an edge coming from node
            for(int nei:adj[node]){
                inDegree[nei]--;
                //Adding it to the queue if the in degree becomes 0
                if(inDegree[nei]==0){
                    q.push(nei);
                }
            }
        }
        // Checking if there is a cycle present or not
        if(ans.size()!=k){
            return {};
        }
        return ans;
    }
    vector<vector<int>> buildMatrix(int k, vector<vector<int>>& rowConditions, vector<vector<int>>& colConditions) {
        // Finding topological sort for both row and col conditions
        vector<int>rowTopo=kahnTopo(k,rowConditions);
        vector<int>colTopo=kahnTopo(k,colConditions);
        // Answer matrix of size kxk initialized with 0's
        vector<vector<int>>ans(k,vector<int>(k,0));
        // If any of the topoplogical sort is empty that there is a cycle present then it is an impossible case
        if(rowTopo.empty()||colTopo.empty()){
            return {};
        }
        vector<int>rowPos(k+1),colPos(k+1);
        // Determing row and col positions wrt thier resp topological sorts
        for(int i=0;i<k;i++){
            rowPos[rowTopo[i]]=i;
            colPos[colTopo[i]]=i;
        }
        // Forming the answer
        for(int i=1;i<=k;i++){
            ans[rowPos[i]][colPos[i]]=i;
        }
        return ans;
    }
};
