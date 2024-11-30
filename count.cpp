
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <climits>  // Required for INT_MAX and INT_MIN

using namespace std;

// Function to check if a point is inside the polygon using the ray-casting method
bool isPointInsidePolygon(int x, int y, const vector<pair<int, int>>& vertices) {
    int n = vertices.size();
    bool inside = false;

    for (int i = 0; i < n; ++i) {
        int x1 = vertices[i].first, y1 = vertices[i].second;
        int x2 = vertices[(i + 1) % n].first, y2 = vertices[(i + 1) % n].second;

        if ((y1 > y) != (y2 > y) && 
            (x < (double)(x2 - x1) * (y - y1) / (double)(y2 - y1) + x1)) {
            inside = !inside;
        }
    }
    return inside;
}

// Function to calculate the minimum number of brush presses needed
int minimumPresses(const vector<pair<int, int>>& vertices, int brushSize) {
    // Determine the bounding box of the polygon
    int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
    for (const auto& vertex : vertices) {
        minX = min(minX, vertex.first);
        maxX = max(maxX, vertex.first);
        minY = min(minY, vertex.second);
        maxY = max(maxY, vertex.second);
    }

    set<pair<int, int>> coveredPoints;
    int presses = 0;

    // Loop over all grid points within the bounding box
    for (int x = minX; x <= maxX; ++x) {
        for (int y = minY; y <= maxY; ++y) {
            // Check if the current point is inside the polygon
            if (isPointInsidePolygon(x, y, vertices)) {
                // Check if this point and its surrounding grid points are not yet covered
                bool needsPress = true;
                for (int dx = 0; dx < brushSize && needsPress; ++dx) {
                    for (int dy = 0; dy < brushSize; ++dy) {
                        if (coveredPoints.count({x + dx, y + dy})) {
                            needsPress = false;
                            break;
                        }
                    }
                }
                // If a press is needed, increment presses and mark points covered
                if (needsPress) {
                    presses++;
                    for (int dx = 0; dx < brushSize; ++dx) {
                        for (int dy = 0; dy < brushSize; ++dy) {
                            coveredPoints.insert({x + dx, y + dy});
                        }
                    }
                }
            }
        }
    }

    return presses;
}

int main() {
    int n;
    cin >> n;

    vector<pair<int, int>> vertices(n);
    for (int i = 0; i < n; ++i) {
        cin >> vertices[i].first >> vertices[i].second;
    }

    int brushSize;
    cin >> brushSize;

    // Output the result with no additional newline or spaces after the output
    cout << minimumPresses(vertices, brushSize);

    return 0;
}