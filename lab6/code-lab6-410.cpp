class Solution {
public:
    // Helper function to check if it's possible to split the array into k subarrays
    // such that the largest sum does not exceed `maxSum`
    bool canSplit(const vector<int>& nums, int k, int maxSum) {
        int current_sum = 0;
        int splits = 1; // Start with one subarray
        
        for (int num : nums) {
            if (current_sum + num > maxSum) {
                splits++;
                current_sum = num;
                if (splits > k) {
                    return false; // More than k subarrays are required
                }
            } else {
                current_sum += num;
            }
        }
        return true;
    }

    // Function to minimize the largest sum by binary search
    int splitArray(vector<int>& nums, int k) {
        int left = *max_element(nums.begin(), nums.end()); // The minimum possible largest sum is the largest element
        int right = accumulate(nums.begin(), nums.end(), 0); // The maximum possible largest sum is the sum of all elements
        int result = right;

        while (left <= right) {
            int mid = left + (right - left) / 2;

            if (canSplit(nums, k, mid)) {
                result = mid;
                right = mid - 1; // Try to minimize the largest sum
            } else {
                left = mid + 1; // Increase the allowed sum
            }
        }

        return result;
    }
};