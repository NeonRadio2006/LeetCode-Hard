// 1.Prim's Algo + Binary Lifting
// Time:- O(ElogE + (V+Q)logV) where E=number of edges,Q=number of queries and V=number of nodes
// Space:- O(VlogV + E + 3V)
class Solution {
public:
    // Function to filter out unwanted edges from the given edgeList 
    vector<vector<pair<int,long long>>>makeAdj(int n,vector<vector<int>>&edgeList){
        // As there can be multiple edges between two adjacent nodes and there can be multiple adjacent nodes
        // temp[x][y] denotes the weight of the edge which is minimum among all the weights of the edges that is present between x and y
        unordered_map<int,unordered_map<int,long long>>temp;
        // adj denotes the filtered list
        vector<vector<pair<int,long long>>>adj(n);
        // Traversing through every edge given in the initial edgeList
        for(auto edge:edgeList){
            // Extract the nodes and the weights
            int u=edge[0];
            int v=edge[1];
            long long w=(long long)edge[2];
            // If this neighbour is encountered for the first time OR the weight present in the temp is greater than the weight of the current edge then assign w to temp[u][v]
            if(!temp[u].count(v)||temp[u][v]>w){
                temp[u][v]=w;
            }
            // If this neighbour is encountered for the first time OR the weight present in the temp is greater than the weight of the current edge then assign w to temp[v][u]
            if(!temp[v].count(u)||temp[v][u]>w){
                temp[v][u]=w;
            }
        }
        // Form the final adjacency list using temp
        for(auto& [u,mp]:temp){
            for(auto& [v,w]:mp){
                adj[u].push_back({v,w});
            }
        }
        // Return the final adjacency list
        return adj;
    }
    // Function to form the MST using Prim's Algo
    vector<vector<pair<int,long long>>>primsMST(int n,vector<vector<pair<int,long long>>>&adj){
        // visited[i] is true if node i is already included in the tree and false otherwise
        // This is done to avoid cycles
        vector<bool>visited(n,false);
        // We can also have to account for disconnected graph where there are multiple components
        // Hence we are taking a parent array
        vector<int>parent(n,-1);
        // Min-Heap, denotes {weight,{current node, parent of current node}}
        priority_queue<pair<long long,pair<int,int>>,vector<pair<long long,pair<int,int>>>,greater<pair<long long,pair<int,int>>>>pq;
        // Final mst that we have to return
        vector<vector<pair<int,long long>>>mst(n);
        // Checking every node from 0 to n-1
        for(int i=0;i<n;i++){
            // If the node i is already visited then ignore it
            if(visited[i]){
                continue;
            }
            // If the node is not visited then it means that we have encounterd a new component
            // Push it to the queue with weight as 0 and parent as -1
            pq.push({0,{i,-1}});
            while(!pq.empty()){
                // Extract the edge with the minimum weight
                auto curr=pq.top();
                // Pop it
                pq.pop();
                // Extract current distance,node and it's parent
                long long currDist=curr.first;
                int currNode=curr.second.first;
                int parentOfCurrNode=curr.second.second;
                // If the current node is already in the tree then ignore it
                if(visited[currNode]){
                    continue;
                }
                // If not in the tree then mark it as visited
                visited[currNode]=true;
                // Assign the parent
                parent[currNode]=parentOfCurrNode;
                // If the current node is not the source of this component then map it in the tree both ways
                if(parentOfCurrNode!=-1){
                    mst[parentOfCurrNode].push_back({currNode,currDist});
                    mst[currNode].push_back({parentOfCurrNode,currDist});
                }
                // Then move to every neighbour of current node and push it into the queue if it is not visited
                for(auto [nei,dist]:adj[currNode]){
                    if(!visited[nei]){
                        pq.push({dist,{nei,currNode}});
                    }
                }
            }
        }
        // Return the final MST
        return mst;
    }
    // DFS code for pre-processing
    void dfs(int node,int parent,long long dist,int comp,vector<int>&depth,vector<int>&component,vector<vector<int>>&up,vector<vector<long long>>&mx,vector<vector<pair<int,long long>>>&mst){
        // Tagging all the nodes of this component with comp
        component[node]=comp;
        // The first ancestor of 'node' will be parent
        up[node][0]=parent;
        // The maximum edge weight between parent and child node is the weight of the direct edge present between them
        mx[node][0]=dist;
        // Go to each node present in the component
        for(auto [child,newDist]:mst[node]){
            if(child==parent){
                continue;
            }
            // Increase the depth by 1 for each recursive call
            depth[child]=depth[node]+1;
            // Call DFS for the child node
            dfs(child,node,newDist,comp,depth,component,up,mx,mst);
        }
    }
    // Function to get the max edge present between nodes u and v
    // As we are in a tree, the only way to get from u to v is to trvel upwards till the LCA of u and v and then travel downwards to v
    // Here we assume that u is deeper than v
    long long getMaxEdge(int u,int v,vector<int>&depth,vector<vector<int>>&up,vector<vector<long long>>&mx){
        int LOG=up[0].size();
        long long ans=0;
        // If depth of v is greater then swap u and v
        if(depth[u]<depth[v]){
            swap(u,v);
        }
        // diff denotes the number of steps that u needs to climb to reach same depth as v
        int diff=depth[u]-depth[v];
        // Levelling depth of u to depth of v
        // Starting from LOG-1, to avoid out of bounds error in 'up' and 'mx' 2D vectors and to also avoid impossible jump
        for(int j=LOG-1;j>=0;j--){
            // Updating the answer accoriding to the binary representation of diff
            if(diff&(1<<j)){
                ans=max(ans,mx[u][j]);
                // Update u
                u=up[u][j];
            }
        }
        // Exiting early if v itself is the LCA
        if(u==v){
            return ans;
        }
        // Jumping u and v together by same size of jumps
        for(int j=LOG-1;j>=0;j--){
            // As both are on same level now, then if the jump is not possible for u then it also not possible for v
            // We jump until the  next jump don't land on the same node
            if(up[u][j]!=-1&&up[u][j]!=up[v][j]){
                // Update the answer wrt both the jumps
                ans=max(ans,mx[u][j]);
                ans=max(ans,mx[v][j]);
                // Update u and v
                u=up[u][j];
                v=up[v][j];
            }
        }
        // Now both u and v are direct children of their lca
        ans=max(ans,mx[u][0]);
        ans=max(ans,mx[v][0]);
        // Return the max edge weight
        return ans;
    }
    vector<bool> distanceLimitedPathsExist(int n, vector<vector<int>>& edgeList, vector<vector<int>>& queries) {
        // Initially we have the raw adjacency list(edgeList), in which there are multiple edges with different weights in between two nodes
        // So we have to first filter the given edge list such that every pair of adjacent nodes have only one edge between them
        // We will greedily keep the edge with the minimum weight between two adjacent nodes so that we always have the optimal choice
        // adj denoted the filtered adjacency list 
        vector<vector<pair<int,long long>>>adj=makeAdj(n,edgeList);
        // Now as we have got the filtered list we need to remove the cycles present in the filtered graph
        // We need minimum number of edges such that the weight of all those edges are as small as possible
        // This is the definition of Minimum Spanning Tree(MST)
        vector<vector<pair<int,long long>>>mst=primsMST(n,adj);
        // Now mst denotes the most optimal set of edges that we could have choose for efficient processing of query
        // Now the main idea is that suppose in the path between node u and v the maximum weight among all the weights of edges presnt in the path is W
        // Then answer will be true iff W<limit for that particualr query
        // Now we need an efficient way to process each query as if we try to find W for every query using BFS/DFS then it would result in a TLE
        // We use Binary Lifting as an efficient approach but first we have to do pre-processing of the tree(MST)
        // This pre-processing will structure the MST in such a way that we will be able to perform O(longn) path queries
        // LOG denotes the maximum power of 2 needed to jump from the deepest node to the root of it's component
        int LOG=1;
        while((1<<LOG)<=n){
            LOG++;
        }
        // up[x][i] denotes the node that is exactly 2ⁱ steps above node x
        vector<vector<int>>up(n,vector<int>(LOG,-1));
        // mx[x][i] denotes the maximum edge weight found on the path between node x and 2ⁱ-th ancestor
        vector<vector<long long>>mx(n,vector<long long>(LOG,0));
        // depth[i] stores how deep is node i
        vector<int>depth(n,0);
        // component[i] denotes the component in which node i is present
        vector<int>component(n,-1);
        // Starting the component tag from 0
        int comp=0;
        for(int i=0;i<n;i++){
            // If node i is already a part of some component then ignore it and move forward
            if(component[i]!=-1){
                continue;
            }
            // Depth of root of this component is 0
            depth[i]=0;
            // Call the dfs to include all the nodes of this component
            dfs(i,-1,0,comp,depth,component,up,mx,mst);
            // Increment the comp tag for next component
            comp++;
        }
        // Binary Lifting Code
        // We are starting with 1 as the first ancestor was calculated in the DFS
        // The jump loop is the outer loop as we have to calculate the 2nd ancestor for every node before calculating the 4th ancestor and so on
        for(int j=1;j<LOG;j++){
            for(int i=0;i<n;i++){
                // If we can't make the jump of half the size then surely we can't make the jump of full size
                if(up[i][j-1]==-1){
                    up[i][j]=-1;
                    mx[i][j]=mx[i][j-1];
                }
                else{
                    // Now we jump halfway
                    up[i][j]=up[up[i][j-1]][j-1];
                    // Now the max edge between node i and j will be max of max edge that is present between i and it's halfway ancestor and the max edge present between the other half 
                    mx[i][j]=max(mx[i][j-1],mx[up[i][j - 1]][j - 1]);
                }
            }
        }
        // Initialize an empty boolean array
        vector<bool>ans;
        // Processing each query
        for(auto& query:queries){
            // Extract the nodes that are to be queries
            int u=query[0];
            int v=query[1];
            // Extract the limit for this particular query
            long long limit=(long long)query[2];
            // If both nodes are from difeerent components then answer is automatically false
            if(component[u]!=component[v]){
                ans.push_back(false);
                continue;
            }
            // Get the max edge between u and v
            long long maxEdge=getMaxEdge(u,v,depth,up,mx);
            // Push back the answer accordingly
            ans.push_back(maxEdge<limit);
        }
        // Return the final answer
        return ans;
    }
};
// 2.DSU
// Time:- O(ElogE + QlogQ)
// Space:- O(V + Q)
// Standard DSU template
class DSU{
    vector<int>parent,rank,sizes;
    int comps;
    public:
    DSU(int n){
        rank.resize(n+1,0);
        parent.resize(n+1);
        sizes.resize(n+1,1);
        comps=n;
        for(int i=0;i<=n;i++){
            parent[i]=i;
        }
    }
    int fpar(int node){
        if(node==parent[node]){
            return node;
        }
        return parent[node]=fpar(parent[node]);
    }
    bool ubr(int u,int v){
        int p_u=fpar(u);
        int p_v=fpar(v);
        if(p_u==p_v){
            return false;
        }
        if(rank[p_u]<rank[p_v]){
            parent[p_u]=p_v;
        }
        else if(rank[p_v]<rank[p_u]){
            parent[p_v]=p_u;
        }
        else{
            parent[p_v]=p_u;
            rank[p_u]++;
        }
        comps--;
        return true;
    }
    bool ubs(int u,int v){
        int p_u=fpar(u);
        int p_v=fpar(v);
        if(p_u==p_v){
            return false;
        }
        if(sizes[p_u]<sizes[p_v]){
            parent[p_u]=p_v;
            sizes[p_v]+=sizes[p_u];
        }
        else{
            parent[p_v]=p_u;
            sizes[p_u]+=sizes[p_v];
        }
        comps--;
        return true;
    }
    bool same(int u,int v){
        return fpar(u)==fpar(v);
    }
    int getsize(int u){
        return sizes[fpar(u)];
    }
    int count(){
        return comps;
    }
};
class Solution {
public:
    vector<bool> distanceLimitedPathsExist(int n, vector<vector<int>>& edgeList, vector<vector<int>>& queries) {
        // Sort the edgeList in the increasing order of the weight
        sort(edgeList.begin(),edgeList.end(),[](vector<int>&a,vector<int>&b){
            return a[2]<b[2];
        });
        // Modify the query structure
        vector<vector<int>>newQueries;
        for(int i=0;i<queries.size();i++){
            newQueries.push_back({queries[i][2],queries[i][0],queries[i][1],i});
        }
        // And sort according to increasing order of limit
        sort(newQueries.begin(),newQueries.end());
        // Initialize the DSU and the answer array
        DSU dsu(n);
        vector<bool>ans(queries.size());
        // pointer to traverse
        int j=0;
        for(auto& query:newQueries){
            int limit=query[0];
            int u=query[1];
            int v=query[2];
            int originalIndex=query[3];
            // Merge the edges
            while(j<edgeList.size()&&edgeList[j][2]<limit){
                dsu.ubs(edgeList[j][0],edgeList[j][1]);
                j++;
            }
            // Now check whether u and v are in same component or not
            ans[originalIndex]=dsu.same(u,v);
        }
        // Return the final answer
        return ans;
    }
};
