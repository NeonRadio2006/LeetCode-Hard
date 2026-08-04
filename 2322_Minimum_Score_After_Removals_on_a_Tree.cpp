// Mistake I was doing was considering evary pair of edges, but instead we should consider pair of nodes because if we remove a node then it will automatically denotes of breaking an edge
class Solution {
public:
    // Checking if one node is descendant of other or not in strictly O(1)
    // EULER TOUR CONCEPT
    bool vIsDescendantOfU(int u,int v,vector<int>& inTime,vector<int>& outTime){
        return inTime[v]>=inTime[u]&&outTime[v]<=outTime[u];
    }
    // DFS for precomputation
    void dfs(int node,int parent,int& timer,vector<int>& xorOfSubRootedAt,vector<int>& nums,vector<int>& inTime,vector<int>& outTime,vector<vector<int>>& adj){
        // Initialze xor of subtree with the element at nums
        xorOfSubRootedAt[node]=nums[node];
        // Assign and increase the timer
        inTime[node]=timer;
        timer++;
        // Hit DFS for connnected nodes
        for(int nei:adj[node]){
            if(nei!=parent){
                dfs(nei,node,timer,xorOfSubRootedAt,nums,inTime,outTime,adj);
                // Perform xor with all the neighbours to find the final xor of the whole subtree
                xorOfSubRootedAt[node]^=xorOfSubRootedAt[nei];
            }
        }
        // Assign and increase the timer
        outTime[node]=timer;
        timer++;
    }
    // Helper function to get score after removing two edges
    int scoreForPair(int u,int v,vector<int>& xorOfSubRootedAt,vector<int>& inTime,vector<int>& outTime){
        // xor's of 3 components
        int xor1,xor2,xor3;
        // If v is descendant of u
        if(vIsDescendantOfU(u,v,inTime,outTime)){
            xor1=xorOfSubRootedAt[v];
            xor2=xorOfSubRootedAt[u]^xor1;
            xor3=xorOfSubRootedAt[0]^xor2^xor1;
        }
        // If u is descendant of v 
        else if(vIsDescendantOfU(v,u,inTime,outTime)){
            xor1=xorOfSubRootedAt[u];
            xor2=xorOfSubRootedAt[v]^xor1;
            xor3=xorOfSubRootedAt[0]^xor1^xor2;
        }
        // If both u and v belobngs to different subtrees
        else{
            xor1=xorOfSubRootedAt[u];
            xor2=xorOfSubRootedAt[v];
            xor3=xorOfSubRootedAt[0]^xor2^xor1;
        }
        // Calculating the final score
        int score=max({xor1,xor2,xor3})-min({xor1,xor2,xor3});
        return score;
    }
    int minimumScore(vector<int>& nums, vector<vector<int>>& edges) {
        int n=nums.size();
        int ans=INT_MAX;
        vector<vector<int>>adj(n);
        // Forming the adj list
        for(auto edge:edges){
            adj[edge[0]].push_back(edge[1]);
            adj[edge[1]].push_back(edge[0]);
        }
        // Vector to store the total xor of a node and it's subtree
        vector<int>xorOfSubRootedAt(n,0);
        // in and out time vector (Euler Tour) to determine ancestors in O(1)
        vector<int>inTime(n,0);
        vector<int>outTime(n,0);
        int timer=0;
        // DFS ( Tree Rooted at 0) with also forming the in and out time vector while calculating xor's
        dfs(0,-1,timer,xorOfSubRootedAt,nums,inTime,outTime,adj);
        // Checking every pair of node (Not considering 0 as it was considered as the main root and hence it does not have a parent) 
        for(int u=1;u<n;u++){
            for(int v=u+1;v<n;v++){
                //Calulating the answer
                ans=min(ans,scoreForPair(u,v,xorOfSubRootedAt,inTime,outTime));
            }
        }
        return ans;
    }
};
