// 1.Pure Recursive Solution
class Solution {
public:
    // Recursive function to find the answer
    int find(int i,int currSum,vector<vector<pair<int,int>>>&options){
        // We have exactly achieved our answer
        if(currSum==0){
            return 0;
        }
        // This denotes that either there are no elements remaining or it is an invalid path
        // Hence returning 1e9
        if(i==options.size()||currSum<0){
            return 1e9;
        }
        // Option 1:-Ignore this nums[i]
        int ans=find(i+1,currSum,options);
        // Option 2:-Take exactly one option from the all possible options for this nums[i]
        // Find minimum among all those options
        for(auto option:options[i]){
            int val=option.first;
            int cost=option.second;
            ans=min(ans,cost+find(i+1,currSum-val,options));
        }
        // Returning the answer
        return ans;
    }
    // The only difference with it's I part is that we can have more options as there is no restriction on the order of multiplication and division
    int minOperations(vector<int>& nums, int sum) {
        // Initialize the size of nums array
        int n=nums.size();
        // To store every possible number which can be formed by nums[i] with it's resp cost
        vector<vector<pair<int,int>>>options(n);
        // Precomputation step using BFS
        for(int i=0;i<n;i++){
            vector<int>dist(5001,1e9);
            queue<int>q;
            dist[nums[i]]=0;
            q.push(nums[i]);
            while(!q.empty()){
                int x=q.front();
                q.pop();
                int cost=dist[x]+1;
                if(2*x<=sum&&dist[2*x]==1e9){
                    dist[2*x]=cost;
                    q.push(2*x);
                }
                if(dist[x/2]==1e9){
                    dist[x/2]=cost;
                    q.push(x/2);
                }
            }
            for(int j=0;j<=sum;j++){
                if(dist[j]!=1e9){
                    options[i].push_back({j,dist[j]});
                }
            }
        }
        // Find the answer recursively
        int ans=find(0,sum,options);
        // If the asnwer is 1e9 that means there is no possible way to do it
        // Hence returning -1
        if(ans==1e9){
            return -1;
        }
        // Returning the answer
        return ans;
    }
};
// 2.Memoized Solution
class Solution {
public:
    // Recursive function to find the answer
    int find(int i,int currSum,vector<vector<pair<int,int>>>&options,vector<vector<int>>&dp){
        // We have exactly achieved our answer
        if(currSum==0){
            return 0;
        }
        // This denotes that either there are no elements remaining or it is an invalid path
        // Hence returning 1e9
        if(i==options.size()||currSum<0){
            return 1e9;
        }
        // If the state dp[i][currSum] is already calculated then immediately return it
        if(dp[i][currSum]!=-1){
            return dp[i][currSum];
        }
        // Option 1:-Ignore this nums[i]
        int ans=find(i+1,currSum,options,dp);
        // Option 2:-Take exactly one option from the all possible options for this nums[i]
        // Find minimum among all those options
        for(auto option:options[i]){
            int val=option.first;
            int cost=option.second;
            ans=min(ans,cost+find(i+1,currSum-val,options,dp));
        }
        // Storing and returning the answer
        return dp[i][currSum]=ans;
    }
    // The only difference with it's I part is that we can have more options as there is no restriction on the order of multiplication and division
    int minOperations(vector<int>& nums, int sum) {
        // Initialize the size of nums array
        int n=nums.size();
        // To store every possible number which can be formed by nums[i] with it's resp cost
        vector<vector<pair<int,int>>>options(n);
        // Precomputation step using BFS
        for(int i=0;i<n;i++){
            vector<int>dist(5001,1e9);
            queue<int>q;
            dist[nums[i]]=0;
            q.push(nums[i]);
            while(!q.empty()){
                int x=q.front();
                q.pop();
                int cost=dist[x]+1;
                if(2*x<=sum&&dist[2*x]==1e9){
                    dist[2*x]=cost;
                    q.push(2*x);
                }
                if(dist[x/2]==1e9){
                    dist[x/2]=cost;
                    q.push(x/2);
                }
            }
            for(int j=0;j<=sum;j++){
                if(dist[j]!=1e9){
                    options[i].push_back({j,dist[j]});
                }
            }
        }
        // Initialize the dp table
        // dp[i][x] represent minimum number of operations required to make sum x from the element i..n-1
        vector<vector<int>>dp(n,vector<int>(sum+1,-1));
        // Find the answer recursively
        int ans=find(0,sum,options,dp);
        // If the asnwer is 1e9 that means there is no possible way to do it
        // Hence returning -1
        if(ans==1e9){
            return -1;
        }
        // Returning the answer
        return ans;
    }
};
// 3.Tabulated Solution
class Solution {
public:
    // The only difference with it's I part is that we can have more options as there is no restriction on the order of multiplication and division
    int minOperations(vector<int>& nums, int sum) {
        // Initialize the size of nums array
        int n=nums.size();
        // To store every possible number which can be formed by nums[i] with it's resp cost
        vector<vector<pair<int,int>>>options(n);
        // Precomputation step using BFS
        for(int i=0;i<n;i++){
            vector<int>dist(5001,1e9);
            queue<int>q;
            dist[nums[i]]=0;
            q.push(nums[i]);
            while(!q.empty()){
                int x=q.front();
                q.pop();
                int cost=dist[x]+1;
                if(2*x<=sum&&dist[2*x]==1e9){
                    dist[2*x]=cost;
                    q.push(2*x);
                }
                if(dist[x/2]==1e9){
                    dist[x/2]=cost;
                    q.push(x/2);
                }
            }
            for(int j=0;j<=sum;j++){
                if(dist[j]!=1e9){
                    options[i].push_back({j,dist[j]});
                }
            }
        }
        // Initialize the dp table
        // dp[i][x] represent minimum number of operations required to make sum x from the element i..n-1
        vector<vector<int>>dp(n+1,vector<int>(sum+1,1e9));
        // Translation of the first base case
        for(int i=0;i<=n;i++){
            dp[i][0]=0;
        }
        // Going from i=n-1 to 0
        for(int i=n-1;i>=0;i--){
            // Trying every possible sum value for this i
            for(int currSum=1;currSum<=sum;currSum++){
                // Option 1:-Ignore this nums[i]
                dp[i][currSum]=dp[i+1][currSum];
                // Option 2:-Take exactly one option from the all possible options for this nums[i]
                // Find minimum among all those options
                for(auto option:options[i]){
                    int val=option.first;
                    if(val>currSum){
                        continue;
                    }
                    int cost=option.second;
                    dp[i][currSum]=min(dp[i][currSum],cost+dp[i+1][currSum-val]);
                }
            }
        }
        // Find the answer recursively
        int ans=dp[0][sum];
        // If the asnwer is 1e9 that means there is no possible way to do it
        // Hence returning -1
        if(ans==1e9){
            return -1;
        }
        // Returning the answer
        return ans;
    }
};
// 4.Space Optimized Solution
class Solution {
public:
    // The only difference with it's I part is that we can have more options as there is no restriction on the order of multiplication and division
    int minOperations(vector<int>& nums, int sum) {
        // Initialize the size of nums array
        int n=nums.size();
        // To store every possible number which can be formed by nums[i] with it's resp cost
        vector<vector<pair<int,int>>>options(n);
        // Precomputation step using BFS
        for(int i=0;i<n;i++){
            vector<int>dist(5001,1e9);
            queue<int>q;
            dist[nums[i]]=0;
            q.push(nums[i]);
            while(!q.empty()){
                int x=q.front();
                q.pop();
                int cost=dist[x]+1;
                if(2*x<=sum&&dist[2*x]==1e9){
                    dist[2*x]=cost;
                    q.push(2*x);
                }
                if(dist[x/2]==1e9){
                    dist[x/2]=cost;
                    q.push(x/2);
                }
            }
            for(int j=0;j<=sum;j++){
                if(dist[j]!=1e9){
                    options[i].push_back({j,dist[j]});
                }
            }
        }
        // Initialize the prev vector
        // prev represents:dp[i+1][x]
        vector<int>prev(sum+1,1e9);
        // Translation of the first base case
        prev[0]=0;
        // Going from i=n-1 to 0
        for(int i=n-1;i>=0;i--){
            // curr represents:dp[i][x]
            vector<int>curr(sum+1,1e9);
            curr[0]=0;
            for(int currSum=1;currSum<=sum;currSum++){
                // Option 1:-Ignore this nums[i]
                curr[currSum]=prev[currSum];
                // Option 2:-Take exactly one option from the all possible options for this nums[i]
                // Find minimum among all those options
                for(auto option:options[i]){
                    int val=option.first;
                    if(val>currSum){
                        continue;
                    }
                    int cost=option.second;
                    curr[currSum]=min(curr[currSum],cost+prev[currSum-val]);
                }
            }
            // Better way to assign curr as prev
            prev=move(curr);
        }
        // Find the answer recursively
        int ans=prev[sum];
        // If the asnwer is 1e9 that means there is no possible way to do it
        // Hence returning -1
        if(ans==1e9){
            return -1;
        }
        // Returning the answer
        return ans;
    }
};
