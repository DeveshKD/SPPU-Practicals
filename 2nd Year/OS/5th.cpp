#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <queue>
#include <climits> // For INT_MAX

using namespace std;

// Function to implement FIFO page replacement
int fifoPageReplacement(const vector<int>& pages, int frameCount) {
    queue<int> frameQueue;
    unordered_map<int, bool> inFrame;
    int pageFaults = 0;

    for (int page : pages) {
        if (inFrame.find(page) == inFrame.end()) {
            // Page fault
            pageFaults++;

            // If frames are full, remove the oldest page
            if (frameQueue.size() == static_cast<size_t>(frameCount)) {
                int oldestPage = frameQueue.front();
                frameQueue.pop();
                inFrame.erase(oldestPage);
            }

            // Add new page to frames
            frameQueue.push(page);
            inFrame[page] = true;
        }
    }

    return pageFaults;
}

// Function to implement LRU page replacement
int lruPageReplacement(const vector<int>& pages, int frameCount) {
    unordered_map<int, int> frameMap;  // page -> last used time
    int pageFaults = 0;
    int time = 0;

    for (int page : pages) {
        time++;

        if (frameMap.find(page) == frameMap.end()) {
            // Page fault
            pageFaults++;

            // If frames are full, remove the least recently used page
            if (frameMap.size() == static_cast<size_t>(frameCount)) {
                int lruPage = -1, minTime = INT_MAX;
                for (const auto& entry : frameMap) {
                    if (entry.second < minTime) {
                        minTime = entry.second;
                        lruPage = entry.first;
                    }
                }
                frameMap.erase(lruPage);
            }
        }

        // Update the page's last used time
        frameMap[page] = time;
    }

    return pageFaults;
}

// Function to implement Optimal page replacement
int optimalPageReplacement(const vector<int>& pages, int frameCount) {
    vector<int> frames;
    int pageFaults = 0;

    for (size_t i = 0; i < pages.size(); ++i) {
        int page = pages[i];

        // Check if page is already in frames
        if (find(frames.begin(), frames.end(), page) == frames.end()) {
            // Page fault
            pageFaults++;

            if (frames.size() < static_cast<size_t>(frameCount)) {
                frames.push_back(page);
            } else {
                // Find the page that won't be used for the longest time in the future
                int indexToReplace = -1, farthestIndex = i;
                for (size_t j = 0; j < frames.size(); ++j) {
                    auto it = find(pages.begin() + i + 1, pages.end(), frames[j]);
                    size_t distance = (it == pages.end()) ? pages.size() : it - pages.begin();

                    if (distance > farthestIndex) {
                        farthestIndex = distance;
                        indexToReplace = j;
                    }
                }

                frames[indexToReplace] = page;
            }
        }
    }

    return pageFaults;
}

int main() {
    int frameCount;
    cout << "Enter the number of frames: ";
    cin >> frameCount;

    int pageCount;
    cout << "Enter the number of pages: ";
    cin >> pageCount;

    vector<int> pages(pageCount);
    cout << "Enter the pages: ";
    for (int i = 0; i < pageCount; ++i) {
        cin >> pages[i];
    }

    int fifoFaults = fifoPageReplacement(pages, frameCount);
    int lruFaults = lruPageReplacement(pages, frameCount);
    int optimalFaults = optimalPageReplacement(pages, frameCount);

    cout << "Page Faults (FIFO): " << fifoFaults << endl;
    cout << "Page Faults (LRU): " << lruFaults << endl;
    cout << "Page Faults (Optimal): " << optimalFaults << endl;

    return 0;
}
