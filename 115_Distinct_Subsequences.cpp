// 1.Recursive Approach
class Solution {
public:
    int find(int i,int j,string&s,string&t){
        // Base Case 1:- If we have traversed whole t that means we have found a probable answer 
        if(j==t.length()){
            return 1;
        }
        // Base Case 2:- If we have traversed whole s that means t is not completed yet but is completed then we will never found answer from here
        if(i==s.length()){
            return 0;
        }
        // If characters at both i and j matches then we have two choices
        // Match these indices and increment both i and j OR
        // Do not mathc the ith character and try to find a different one
        // Add answers form both the possibilities
        if(s[i]==t[j]){
            return find(i+1,j+1,s,t)+find(i+1,j,s,t);
        }
        // If the characters don't match then just increment i
        return find(i+1,j,s,t);
    }
    int numDistinct(string s, string t) {
        // If s is shorter than t,then answer will be 0
        if(s.length()<t.length()){
            return 0;
        }
        // Return the answer
        return find(0,0,s,t);
    }
};
// 2.Memoized Approach
class Solution {
public:
    int find(int i,int j,string&s,string&t,vector<vector<int>>&dp){
        // Base Case 1:- If we have traversed whole t that means we have found a probable answer 
        if(j==t.length()){
            return 1;
        }
        // Base Case 2:- If we have traversed whole s that means t is not completed yet but is completed then we will never found answer from here
        if(i==s.length()){
            return 0;
        }
        // If this state is already calculated then return it directly
        if(dp[i][j]!=-1){
            return dp[i][j];
        }
        // If characters at both i and j matches then we have two choices
        // Match these indices and increment both i and j OR
        // Do not mathc the ith character and try to find a different one
        // Add answers form both the possibilities
        if(s[i]==t[j]){
            // Return and store
            return dp[i][j]=find(i+1,j+1,s,t,dp)+find(i+1,j,s,t,dp);
        }
        // If the characters don't match then just increment i
        // Return and store
        return dp[i][j]=find(i+1,j,s,t,dp);
    }
    int numDistinct(string s, string t) {
        int n=s.length();
        int m=t.length();
        // If s is shorter than t,then answer will be 0
        if(n<m){
            return 0;
        }
        // Make 2D-DP array
        vector<vector<int>>dp(n,vector<int>(m,-1));
        // Return the answer
        return find(0,0,s,t,dp);
    }
};
// 3.Tabulated Approach
class Solution {
public:
    int numDistinct(string s, string t) {
        int n=s.length();
        int m=t.length();
        // If s is shorter than t,then answer will be 0
        if(n<m){
            return 0;
        }
        // Make 2D-DP array
        // Unsigned long long prevents strict C++ crashes on intermediate overflows
        vector<vector<unsigned long long>>dp(n+1,vector<unsigned long long>(m+1,0));
        // Translation of Base Case 1
        for(int i=0;i<=n;i++){
            dp[i][m]=1;
        }
        // Translation of Base Case 2
        // Remember to not do j<=m as we have already assigned it dp[n][m] as 1 in the above loop
        // We don't have to explicitly write this base case as we have already assigne dp table with 0
        //for(int j=0;j<m;j++){
            //dp[n][j]=0;
        //}
        // As we need i+1 and j+1 for i and j so we will traverse backwards with both i and j
        for(int i=n-1;i>=0;i--){
            for(int j=m-1;j>=0;j--){
                // Direct translation of recursion
                if(s[i]==t[j]){
                    dp[i][j]=dp[i+1][j+1]+dp[i+1][j];
                }
                else{
                    dp[i][j]=dp[i+1][j];
                }
            }
        }
        // Returning the answer
        return (int)dp[0][0];
    }
};
// 4.Space Optimized Approach
class Solution {
public:
    int numDistinct(string s, string t) {
        int n=s.length();
        int m=t.length();
        // If s is shorter than t,then answer will be 0
        if(n<m){
            return 0;
        }
        // Make 2D-DP array
        // Unsigned long long prevents strict C++ crashes on intermediate overflows
        // prev denotes dp[i+1][...]
        // curr denotes dp[i][...]
        vector<unsigned long long>prev(m+1,0);
        vector<unsigned long long>curr(m+1,0);
        // Translation of Base Case 1
        prev[m]=1;
        // As we need i+1 and j+1 for i and j so we will traverse backwards with both i and j
        for(int i=n-1;i>=0;i--){
            // Translation of Base Case 1
            curr[m]=1;
            for(int j=m-1;j>=0;j--){
                // Direct translation of recursion
                if(s[i]==t[j]){
                    curr[j]=prev[j+1]+prev[j];
                }
                else{
                    curr[j]=prev[j];
                }
            }
            prev=curr;
        }
        // Returning the answer
        return (int)prev[0];
    }
};
