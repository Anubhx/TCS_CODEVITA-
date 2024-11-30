#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
#include <functional> // Included for std::function

using namespace std;

struct Point {
    double x, y;
    int id;

    Point(double x = 0, double y = 0, int id = -1) : x(x), y(y), id(id) {}

    bool operator<(const Point& other) const {
        if (fabs(x - other.x) > 1e-9) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Point& other) const {
        return fabs(x - other.x) < 1e-9 && fabs(y - other.y) < 1e-9;
    }
};

struct Segment {
    Point p1, p2;
};

int N;
vector<Point> points;
vector<Segment> segments;
map<pair<int, int>, vector<int>> adj;

double cross(const Point& O, const Point& A, const Point& B) {
    return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}

bool onSegment(const Point& p, const Point& q, const Point& r) {
    return q.x <= max(p.x, r.x) + 1e-9 && q.x >= min(p.x, r.x) - 1e-9 &&
           q.y <= max(p.y, r.y) + 1e-9 && q.y >= min(p.y, r.y) - 1e-9;
}

bool segmentsIntersect(Point p1, Point q1, Point p2, Point q2) {
    double o1 = cross(p1, q1, p2);
    double o2 = cross(p1, q1, q2);
    double o3 = cross(p2, q2, p1);
    double o4 = cross(p2, q2, q1);

    if (o1 * o2 < -1e-9 && o3 * o4 < -1e-9)
        return true;

    if (fabs(o1) < 1e-9 && onSegment(p1, p2, q1)) return true;
    if (fabs(o2) < 1e-9 && onSegment(p1, q2, q1)) return true;
    if (fabs(o3) < 1e-9 && onSegment(p2, p1, q2)) return true;
    if (fabs(o4) < 1e-9 && onSegment(p2, q1, q2)) return true;

    return false;
}

Point getIntersection(Point p1, Point p2, Point p3, Point p4) {
    double A1 = p2.y - p1.y;
    double B1 = p1.x - p2.x;
    double C1 = A1 * p1.x + B1 * p1.y;

    double A2 = p4.y - p3.y;
    double B2 = p3.x - p4.x;
    double C2 = A2 * p3.x + B2 * p3.y;

    double det = A1 * B2 - A2 * B1;
    if (fabs(det) < 1e-9) {
        return Point();
    } else {
        double x = (B2 * C1 - B1 * C2) / det;
        double y = (A1 * C2 - A2 * C1) / det;
        return Point(x, y);
    }
}

int main() {
    cin >> N;

    int point_id = 0;
    map<Point, int> point_ids;
    for (int i = 0; i < N; ++i) {
        int x1, y1, x2, y2;
        cin >> x1 >> y1 >> x2 >> y2;

        Point p1(x1, y1), p2(x2, y2);

        if (point_ids.find(p1) == point_ids.end()) {
            p1.id = point_id;
            point_ids[p1] = point_id++;
            points.push_back(p1);
        } else {
            p1.id = point_ids[p1];
        }

        if (point_ids.find(p2) == point_ids.end()) {
            p2.id = point_id;
            point_ids[p2] = point_id++;
            points.push_back(p2);
        } else {
            p2.id = point_ids[p2];
        }

        segments.push_back({p1, p2});
    }

    for (size_t i = 0; i < segments.size(); ++i) {
        for (size_t j = i + 1; j < segments.size(); ++j) {
            if (segmentsIntersect(segments[i].p1, segments[i].p2, segments[j].p1, segments[j].p2)) {
                Point ip = getIntersection(segments[i].p1, segments[i].p2, segments[j].p1, segments[j].p2);

                if (point_ids.find(ip) == point_ids.end()) {
                    ip.id = point_id;
                    point_ids[ip] = point_id++;
                    points.push_back(ip);
                } else {
                    ip.id = point_ids[ip];
                }
            }
        }
    }

    map<int, vector<int>> adjList;
    for (auto& seg : segments) {
        vector<Point> pts_on_seg;
        for (auto& pt : points) {
            if (onSegment(seg.p1, pt, seg.p2) && fabs(cross(seg.p1, seg.p2, pt)) < 1e-9) {
                pts_on_seg.push_back(pt);
            }
        }

        sort(pts_on_seg.begin(), pts_on_seg.end(), [&](const Point& a, const Point& b) {
            double da = hypot(a.x - seg.p1.x, a.y - seg.p1.y);
            double db = hypot(b.x - seg.p1.x, b.y - seg.p1.y);
            return da < db;
        });

        for (size_t i = 0; i < pts_on_seg.size() - 1; ++i) {
            int u = pts_on_seg[i].id;
          int v = pts_on_seg[i + 1].id;
            if (u != v) {
                adjList[u].push_back(v);
                adjList[v].push_back(u);
            }
        }
    }

    set<vector<int>> cycles;
    int maxArea = 0;

    function<void(int, int, vector<int>&, set<int>&)> dfs =
        [&](int start, int u, vector<int>& path, set<int>& visited) {
            visited.insert(u);
            path.push_back(u);

            for (int v : adjList[u]) {
                if (v == start && path.size() >= 3) {
                    vector<int> cycle = path;
                    int min_idx = min_element(cycle.begin(), cycle.end()) - cycle.begin();
                    rotate(cycle.begin(), cycle.begin() + min_idx, cycle.end());
                    if (cross(points[cycle[0]], points[cycle[1]], points[cycle[2]]) < 0) {
                        reverse(cycle.begin(), cycle.end());
                    }
                    cycles.insert(cycle);
                } else if (visited.find(v) == visited.end()) {
                    dfs(start, v, path, visited);
                }
            }
            visited.erase(u);
            path.pop_back();
        };

    for (size_t i = 0; i < points.size(); ++i) {
        vector<int> path;
        set<int> visited;
        dfs(i, i, path, visited);
    }

    for (auto& cycle : cycles) {
        double area = 0;
        int sz = cycle.size();
        for (int i = 0; i < sz; ++i) {
            Point& p1 = points[cycle[i]];
            Point& p2 = points[cycle[(i + 1) % sz]];
            area += (p1.x * p2.y - p2.x * p1.y);
        }
        area = fabs(area) / 2.0;
        maxArea = max(maxArea, static_cast<int>(area + 0.5));
    }

    cout << maxArea;

    return 0;
}