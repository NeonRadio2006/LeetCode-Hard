// 1.Brute Force Approach
// Time:- O(N + (N * Qq)) [N denotes number of nodes and Qq denotes number of path queries]
// Space: O(N + Qq) [N denotes number of nodes and Qq denotes number of path queries]
// TLE
class Solution {
public:
    // Bactracking function to find the path between nodes u and v
    bool find(int u,int v,int parent,string&s,string&path,vector<vector<int>>&adj){
        // Push the current character(that is s[u])
        path.push_back(s[u]);
        // If we have reached v then return true
        if(u==v){
            return true;
        }
        // Go to every neighbour of this node
        for(int nei:adj[u]){
            // Except the parent
            if(nei!=parent){
                // And return true if this neighbour lead us to v
                if(find(nei,v,u,s,path,adj)){
                    return true;
                }
            }
        }
        // If not then just pop back this character(Backtrack)
        path.pop_back();
        // And return false
        return false;
    }
    // Function to determine whether a string can be arranged to form a palindrome or not
    bool canFormPalindrome(string& path){
        // Initialize the frequency of each character as 0
        vector<int>freq(26,0);
        // Denotes the number of characters with odd frequencies
        int countOfOddFreq=0;
        // For each character in the path
        for(char c:path){
            // Increment it's frequency
            freq[c-'a']++;
            // Then after incrementing:-
            // If the frequency of c is odd, then this means we have to increment countOfOddFreq
            if(freq[c-'a']%2){
                countOfOddFreq++;
            }
            // Otherwise we have to decrement countOfOddFreq
            else{
                countOfOddFreq--;
            }
        }
        // For characters of path to be able to form a palindrome the number of characters with odd frequency should be atmost 1
        return countOfOddFreq<=1;
    }
    vector<bool> palindromePath(int n, vector<vector<int>>& edges, string s, vector<string>& queries) {
        // Initialize the adjacency list
        vector<vector<int>>adj(n);
        // Form the adjacency list
        for(auto& edge:edges){
            int u=edge[0];
            int v=edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        // Initialize the answer vector
        vector<bool>ans;
        // For each query:-
        for(auto& query:queries){
            // Determine the index where the first space is present
            int firstSpace=query.find(' ');
            // Determine the index where the second space is present 
            int secondSpace=query.find(' ',firstSpace+1);
            // Determine the first node of the query
            int u=stoi(query.substr(firstSpace+1,secondSpace-firstSpace-1));
            // If the query is of the update type:-
            if(query[0]=='u'){
                // The character will always be the last character of the query string
                char c=query.back();
                // Update the character
                s[u]=c;
            }
            // Otherwise:-
            else{ 
                // Determine the second node of the query
                int v=stoi(query.substr(secondSpace+1));
                // Initialize a empty string
                string path="";
                // Find the string that the path between nodes u and v form using recursion
                find(u,v,-1,s,path,adj);
                // Push the answer accordingly
                ans.push_back(canFormPalindrome(path));
            }
        }
        // Returning the final answer
        return ans;
    }
};
// 2.Optmized Approach
// Time:- O((N + Q)logN) [N denotes number of nodes and Q denotes number of queries]
// Space: O(NlogN) [N denotes number of nodes]
// AC

// 1st Optmization:-
// Instead of forming the whole path as a string we can just determine the number of characters which appears odd number of times as the the exact characters are not important 
// To determine this we use XOR 
// We assign a->00..01, b->00..10, c->00..100, and so on
// And we will do this by using (1<<(ch-'a))
// Whenever we encounter a character we will flip it's respective bit
// And at the end if there are more than 1 set bits in the final mask then it is not possible to rearrange the character into a palindrome
// To check if atmost one bit is set or not we will check if (mask & (mask-1))==0 is true or not

// 2nd Optimization:-
// To avoid the repetitive recursive calls we can set any node as root(0 for now)
// Then for every node u we will find mask for all the characters that appears on the path from 0 to u, we will store this in a seperate vector
// Then for the mask of path from node u to node v we can take xor of mask for the path from 0 to u and mask for the path from 0 to v
// Doing this will cancel out the characters which are common in both the paths but are not in the path between u and v
// Those common characters are the characters from 0 to LCA of u and v 
// But this also cancels out the LCA which is in the path
// Hence, we also have to xor the LCA of u and v seperately
// So the final mask of the path between node u and node v becomes [0...u]^[0...v]^LCA(u,v)
// We can use Binary Lifting to efficiently find the LCA of node u and node v

// 3rd Optimization:-
// We have optimized the path query but we now have complicated the update queries
// When we apply an update query we would have to recalculate all the values of mask(path from 0 to node u) again which would again take O(N) which will at the end again lead us to TLE
// To solve this we will flatten the tree, that is apply Euler Tour
// We record the in(tin) and out time(tout) for each node and also record the node in which we were at a particular time in a seperate eulwr tou array
// Now the mask(path from 0 to u) is the xor sum from time 0 to tin[u].Why?
// Bcz in the euler tour array every node is recorded is twice, that is once at time tin[u] and once at time tout[u]
// So even if the DFS visits other node before u which are not in the path from 0 to u they will get cancelled out in the xor sum as they are recorded twice

// 4th Optimization:-
// Even after flattening tree we still have to appply two operations on the euler tour array
// First one is that range queries which is to find xor sum from time 0 to tin[u]
// And the second one is point updates at tin[u] and tout[u] when there is an update query
// If we just use the euler tour array the query of second type will take O(1) but the query of first type will still take O(N) per query
// To solve this we can make a prefix xor array which will make the query of first type answer in O(1) 
// But if there is an update and after applying the update we have to recalculate the prefix xor which will again lead us to O(N) per query
// To fix this we build a Segement tree over the euler tour array which will male both the type of query answer in O(logN)
class Solution {
public:
    // DFS function for preprocessing of euler tour and binary lifting
    void dfs(int u,int p,int d,string&s,int& time,vector<int>&tin,vector<int>&tout,vector<int>&euler,vector<int>&depth,vector<vector<int>>&up,vector<vector<int>>&adj){
        // Update the timer and assign it
        time=time+1;
        tin[u]=time;
        euler[time]=(1<<(s[u]-'a'));
        // Form the up tabel for binary lifting
        up[u][0]=(p==-1)?u:p;
        for(int i=1;i<20;i++){
            up[u][i]=up[up[u][i-1]][i-1];
        }
        // Assign the depth of this node
        depth[u]=d;
        // Go to every unvisited neighbour of this node u
        for(int v:adj[u]){
            if(v!=p){
                dfs(v,u,d+1,s,time,tin,tout,euler,depth,up,adj);
            }
        }
        // Update the timer and assign it
        time=time+1;
        tout[u]=time;
        euler[time]=(1<<(s[u]-'a'));
    }
    // Function to build segment tree
    // node denotes the current index inside the segTree vector
    // start denotes the left boundary of the segment in the euler array that this node is responsible for
    // end denotes the right boundary of the segment in the euler array that this node is responsible for
    void buildSegTree(int node,int start,int end,vector<int>&segTree,vector<int>&euler){
        // If we have reached leaf
        if(start==end){
            segTree[node]=euler[start];
            return;
        }
        // Find the mid
        int mid=start+(end-start)/2;
        // Build the segment tree for the left half
        buildSegTree(2*node+1,start,mid,segTree,euler);
        // Build the segment tree for the right half
        buildSegTree(2*node+2,mid+1,end,segTree,euler);
        // Combine the result of both the halves
        segTree[node]=segTree[2*node+1]^segTree[2*node+2];
    }
    // Function to update the segment tree after applying update query
    // idx denotes the exact index in the euler array that need to be updated
    // val denotes the xor bitmask(the new character) that will replace the old value at idx
    void updateSegTree(int node,int start,int end,int idx,int val,vector<int>&segTree){
        // If we have arrived at the index where we need to perform the update operation
        if(start==end){
            segTree[node]=val;
            return;
        }
        // Find the mid
        int mid=start+(end-start)/2;
        // Determin in which half does idx lies
        if(start<=idx&&idx<=mid){
            updateSegTree(2*node+1,start,mid,idx,val,segTree);
        }
        else{
            updateSegTree(2*node+2,mid+1,end,idx,val,segTree);
        }
        // After all the recursive calls update the tree
        segTree[node]=segTree[2*node+1]^segTree[2*node+2];
    }
    // Function to find xor sum of a range
    // l denotes the left boundary of target query range
    // r denotes the right boundary of target query range
    int querySegTree(int node,int start,int end,int l,int r,vector<int>&segTree){
        // Scenario 1:- The node's chunk is completely outside
        if(r<start|| end<l){
            return 0;
        }
        // Scenario 2:- Complete inclusion
        if(l<=start&&end<=r){
            return segTree[node];
        }
        // Scenario 3:Partial overlap
        int mid=start+(end-start)/2;
        int leftXor=querySegTree(2*node+1,start,mid,l,r,segTree);
        int rightXor=querySegTree(2*node+2,mid+1,end,l,r,segTree);
        return leftXor^rightXor;
    }
    // Function to find LCA of two nodes using binary lifting
    int findLCA(int u,int v,vector<int>&depth,vector<vector<int>>&up){
        // We always assume node u to be deeper
        if(depth[u]<depth[v]){
            swap(u,v);
        }
        // Calculating the difference between the depths of two nodes
        int diff=depth[u]-depth[v];
        // Phase 1:- Levelling both the nodes
        for(int i=0;i<20;i++){
            if((diff>>i)&1){
                u=up[u][i];
            }
        }
        // Once levelled if the nodes are equal that means one of the nodes was the LCA itself. So return it immediately
        if(u==v){
            return u;
        }
        // Phase 2:- Finding the LCA by shifting both the nodes updwards simultaneously
        for(int i=19;i>=0;i--){
            if(up[u][i]!=up[v][i]){
                u=up[u][i];
                v=up[v][i];
            }
        }
        // At the end both the nodes will become children of thier LCA
        // Hence return the immediate parent
        return up[u][0];
    }
    vector<bool> palindromePath(int n, vector<vector<int>>& edges, string s, vector<string>& queries) {
        // Initialize all the required vectors
        vector<vector<int>>adj(n);
        vector<vector<int>>up(n,vector<int>(20,-1));
        vector<int>tin(n,0);
        vector<int>tout(n,0);
        vector<int>depth(n,0);
        vector<int>euler(2*n+1,0);
        vector<int>segTree(8*n+1,0);
        int time=0;
        // Form the adjacency list
        for(auto& edge:edges){
            int u=edge[0];
            int v=edge[1];
            adj[u].push_back(v);
            adj[v].push_back(u);
        }
        // Initialize the dfs for preprocessing
        dfs(0,-1,0,s,time,tin,tout,euler,depth,up,adj);
        // Build the segment tree
        buildSegTree(0,1,2*n,segTree,euler);
        // Initialize the answer vector
        vector<bool>ans;
        // For each query:-
        for(auto& query:queries){
            // Determine the index where the first space is present
            int firstSpace=query.find(' ');
            // Determine the index where the second space is present 
            int secondSpace=query.find(' ',firstSpace+1);
            // Determine the first node of the query
            int u=stoi(query.substr(firstSpace+1,secondSpace-firstSpace-1));
            // If the query is of the update type:-
            if(query[0]=='u'){
                // The character will always be the last character of the query string
                char c=query.back();
                // Update the character
                s[u]=c;
                // Generate the mask accoridng to it
                int newMask=(1<<(s[u]-'a'));
                // Point updates with root at node 0
                updateSegTree(0,1,2*n,tin[u],newMask,segTree);
                updateSegTree(0,1,2*n,tout[u],newMask,segTree);
            }
            // Otherwise:-
            else{ 
                // Determine the second node of the query
                int v=stoi(query.substr(secondSpace+1));
                // Find the LCA of node u and v
                int lca=findLCA(u,v,depth,up);
                // Find the bitmask representing the new character
                int lcaMask=(1<<(s[lca]-'a'));
                // Calculate the final mask of the path
                int finalMask=querySegTree(0,1,2*n,1,tin[u],segTree)^querySegTree(0,1,2*n,1,tin[v],segTree)^lcaMask;
                // Push the answer accordingly
                ans.push_back((finalMask&(finalMask-1))==0);
            }
        }
        // Returning the final answer
        return ans;
    }
};
