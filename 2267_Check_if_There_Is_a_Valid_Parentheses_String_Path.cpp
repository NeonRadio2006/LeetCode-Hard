// 1.Recursive Approach
// Time ≈ O(2⁽ᵐ⁺ⁿ⁾)
// Space:- O(m+n)
// Space is due to recursion stack
// TLE
class Solution {
public:
    // "unMatchedOpenBrackets" denotes the number of opening brackets still remaining which are not matched with a closing one
    bool find(int i,int j,int unMatchedOpenBrackets,vector<vector<char>>& grid){
        // If the current one is an opening then just increment unMatchedOpenBrackets
        if(grid[i][j]=='('){
            unMatchedOpenBrackets++;
        }
        // If the current one is a closing:-
        else{
            // If there exist no unmatcehd opening brackets, then this path is invalid
            if(unMatchedOpenBrackets==0){
                return false;
            }
            // Else just decrement unMatchedOpenBrackets
            unMatchedOpenBrackets--;
        }
        // Base Case:-
        // If we have reached the last cell:-
        if(i==grid.size()-1&&j==grid[0].size()-1){
            // Then return true iff there exist no remaining opening brackets
            if(unMatchedOpenBrackets==0){
                return true;
            }
            // Otherwise return false
            return false;
        }
        // Try to see whether it is possible from this cell or not
        // Perform bitwise OR between the answers from both the down and right direction as we can get a valid path from any of the direction
        bool ans=false;
        // Move down if possible
        if(i+1<grid.size()){
            ans|=find(i+1,j,unMatchedOpenBrackets,grid);
        }
        // Move right if possible
        if(j+1<grid[0].size()){
            ans|=find(i,j+1,unMatchedOpenBrackets,grid);
        }
        // Return the answer
        return ans;
    }
    bool hasValidPath(vector<vector<char>>& grid) {
        // If the grid starts from a closing bracket then it is impossible
        // Immediately return false
        if(grid[0][0]==')'){
            return false;
        }
        // Return the final answer
        return find(0,0,0,grid);
    }
};
// 2.Memoized Approach
// Time:- O(mn*(m+n))
// Space:- O((m+n)(mn+1))
// Space is due to recursion stack and dp the array
// Accepted
class Solution {
public:
    // "unMatchedOpenBrackets" denotes the number of opening brackets still remaining which are not matched with a closing one
    bool find(int i,int j,int unMatchedOpenBrackets,vector<vector<char>>& grid,vector<vector<vector<int>>>&dp){
        // If the current one is an opening then just increment unMatchedOpenBrackets
        if(grid[i][j]=='('){
            unMatchedOpenBrackets++;
        }
        // If the current one is a closing:-
        else{
            // If there exist no unmatcehd opening brackets, then this path is invalid
            if(unMatchedOpenBrackets==0){
                return false;
            }
            // Else just decrement unMatchedOpenBrackets
            unMatchedOpenBrackets--;
        }
        // Base Case:-
        // If we have reached the last cell:-
        if(i==grid.size()-1&&j==grid[0].size()-1){
            // Then return true iff there exist no remaining opening brackets
            if(unMatchedOpenBrackets==0){
                return true;
            }
            // Otherwise return false
            return false;
        }
        // If this state is already calculated the just imeediately return it
        if(dp[i][j][unMatchedOpenBrackets]!=-1){
            return dp[i][j][unMatchedOpenBrackets];
        }
        // Try to see whether it is possible from this cell or not
        // Perform bitwise OR between the answers from both the down and right direction as we can get a valid path from any of the direction
        bool ans=false;
        // Move down if possible
        if(i+1<grid.size()){
            ans|=find(i+1,j,unMatchedOpenBrackets,grid,dp);
        }
        // Move right if possible
        if(j+1<grid[0].size()){
            ans|=find(i,j+1,unMatchedOpenBrackets,grid,dp);
        }
        // Return the answer
        return dp[i][j][unMatchedOpenBrackets]=ans;
    }
    bool hasValidPath(vector<vector<char>>& grid) {
        // If the grid starts from a closing bracket then it is impossible
        // Immediately return false
        if(grid[0][0]==')'){
            return false;
        }
        // Determine the size of the grid
        int m=grid.size();
        int n=grid[0].size();
        // Initialize the dp vector
        vector<vector<vector<int>>>dp(m,vector<vector<int>>(n,vector<int>(m+n,-1)));
        // Return the final answer
        return find(0,0,0,grid,dp);
    }
};
// 3.Tabulated Approach
// Time:- O(mn*(m+n))
// Space:- O(mn*(m+n))
// Space is due to dp the array
// Accepted
class Solution {
public:
    bool hasValidPath(vector<vector<char>>& grid) {
        // Determine the size of the grid
        int m=grid.size();
        int n=grid[0].size();
        // If the grid starts from a closing bracket then it is impossible
        // If the grid ends with a opening brakcet then also it is impossible
        // Immediately return false
        if(grid[0][0]==')'||grid[m-1][n-1]=='('){
            return false;
        }
        // Initialize the dp vector
        vector<vector<vector<bool>>>dp(m,vector<vector<bool>>(n,vector<bool>(m+n,false)));
        // Traverse backwards
        for(int i=m-1;i>=0;i--){
            for(int j=n-1;j>=0;j--){
                // Try every possible number of unmatched opening brackets
                for(int k=0;k<m+n;k++){
                    int unMatchedOpenBrackets=k;
                    // If the bracket at current cell is an opening one the increment unMatchedOpenBrackets
                    if(grid[i][j]=='('){
                        unMatchedOpenBrackets++;
                    }
                    // Else if the current one is a closing:-
                    else{
                        // If unMatchedOpenBrackets is already 0 then it is an invalid path
                        if(unMatchedOpenBrackets==0){
                            dp[i][j][k]=false;
                            continue;
                        }
                        // Otherwise just decrement unMatchedOpenBrackets
                        unMatchedOpenBrackets--;
                    }
                    // If unMatchedOpenBrackets exceeds m+n then already it is an invlaid path
                    if(unMatchedOpenBrackets>=m+n){
                        dp[i][j][k]=false;
                        continue;
                    }
                    // Base Case:-
                    // If we have reached the last cell:-
                    if(i==m-1&&j==n-1){
                        // Then return true iff there exist no remaining opening brackets
                        if(unMatchedOpenBrackets==0){
                            dp[i][j][k]=true;
                        }
                        else{
                            dp[i][j][k]=false;
                        }
                        continue;
                    }
                    bool ans=false;
                    // Move down if possible
                    if(i+1<grid.size()){
                        ans|=dp[i+1][j][unMatchedOpenBrackets];
                    }
                    // Move right if possible
                    if(j+1<grid[0].size()){
                        ans|=dp[i][j+1][unMatchedOpenBrackets];
                    }
                    // Assign the answer
                    dp[i][j][k]=ans;
                }
            }
        }
        // Return the final answer
        return dp[0][0][0];
    }
};
