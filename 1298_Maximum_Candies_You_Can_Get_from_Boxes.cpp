class Solution {
public:
    int maxCandies(vector<int>& status,vector<int>& candies,vector<vector<int>>& keys,vector<vector<int>>& containedBoxes,vector<int>& initialBoxes) {
        // Initialize the answer
        int ans=0;
        // Unordered sets to keep track of boxes which are already opened,currently closed and the keys which are available to us
        unordered_set<int>alreadyOpened,currentlyClosed,availableKeys;
        // Queue to store the boxes which are initially opened
        queue<int>currentlyOpen;
        // The boxes with status as 1 are initially open
        // The boxes with status as 0 are initially closed
        for(int box:initialBoxes){
            if(status[box]){
                currentlyOpen.push(box);
            }
            else{
                currentlyClosed.insert(box);
            }
        }
        // Now checking boxes from the queue
        while(!currentlyOpen.empty()){
            // Current Box
            int box=currentlyOpen.front();
            currentlyOpen.pop();
            // If the box has been already opened then just ignore it
            if(alreadyOpened.find(box)!=alreadyOpened.end()){
                continue;
            }
            // If not then mark this box as already opened
            alreadyOpened.insert(box);
            // Add the candies of the box to our answer
            ans+=candies[box];
            // Analyzing the keys of the boxes which we get from this box
            for(int i=0;i<keys[box].size();i++){
                // If we have found box which is currently closed but can be opened from this key then we will simply use this key
                if(currentlyClosed.find(keys[box][i])!=currentlyClosed.end()){
                    // Add this box in the queue of currently open boxes
                    currentlyOpen.push(keys[box][i]);
                    // And erase it from currently close boxes
                    currentlyClosed.erase(keys[box][i]);
                }
                // If no box is found the store this key as available key for future use
                else{
                    availableKeys.insert(keys[box][i]);
                }
            }
            // Analyzing the boxes which we get from this box
            for(int i=0;i<containedBoxes[box].size();i++){
                // If we found a key which is currently available and can open this box then w can use that key
                if(availableKeys.find(containedBoxes[box][i])!=availableKeys.end()){
                    // Push that box as open box
                    currentlyOpen.push(containedBoxes[box][i]);
                    // Erase that key
                    availableKeys.erase(containedBoxes[box][i]);
                }
                else{
                    // If the status of box is one then also we can push this to currently open ones
                    if(status[containedBoxes[box][i]]){
                        currentlyOpen.push(containedBoxes[box][i]);
                    }
                    // At the end just insert this box in the list of closed boxes foe future use
                    else{
                        currentlyClosed.insert(containedBoxes[box][i]);
                    }
                }
            }
        }
        // Return the answer
        return ans;
    }
};
