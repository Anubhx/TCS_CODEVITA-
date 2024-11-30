import java.util.*;

public class MaximumArea {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Read the number of line segments
        int N = scanner.nextInt();

        // List to store the endpoints of each line segment
        List<int[]> segments = new ArrayList<>();
        for (int i = 0; i < N; i++) {
            int x1 = scanner.nextInt();
            int y1 = scanner.nextInt();
            int x2 = scanner.nextInt();
            int y2 = scanner.nextInt();
            segments.add(new int[]{x1, y1, x2, y2});
        }

        // Use a set of points to store all vertices of the polygon
        Set<String> points = new HashSet<>();
        for (int[] segment : segments) {
            points.add(segment[0] + "," + segment[1]);
            points.add(segment[2] + "," + segment[3]);
        }

        // Sort the points counterclockwise to form a closed polygon
        List<int[]> sortedPoints = sortPoints(points);

        // Calculate the maximum area using the Shoelace formula
        int maxArea = calculateArea(sortedPoints);

        // Print the result
        System.out.println(maxArea);

        scanner.close();
    }

    // Sort points in counterclockwise order
    private static List<int[]> sortPoints(Set<String> points) {
        List<int[]> pointList = new ArrayList<>();
        for (String point : points) {
            String[] split = point.split(",");
            pointList.add(new int[]{Integer.parseInt(split[0]), Integer.parseInt(split[1])});
        }

        // Find centroid of all points
        double cx = 0, cy = 0;
        for (int[] point : pointList) {
            cx += point[0];
            cy += point[1];
        }
        cx /= pointList.size();
        cy /= pointList.size();

        // Make cx and cy effectively final by assigning to final variables
        final double centerX = cx;
        final double centerY = cy;

        // Sort points by angle with respect to the centroid
        pointList.sort((a, b) -> {
            double angleA = Math.atan2(a[1] - centerY, a[0] - centerX);
            double angleB = Math.atan2(b[1] - centerY, b[0] - centerX);
            return Double.compare(angleA, angleB);
        });

        return pointList;
    }

    // Calculate the area of the polygon using Shoelace formula
    private static int calculateArea(List<int[]> polygon) {
        int n = polygon.size();
        int area = 0;

        for (int i = 0; i < n; i++) {
            int[] p1 = polygon.get(i);
            int[] p2 = polygon.get((i + 1) % n);
            area += p1[0] * p2[1] - p2[0] * p1[1];
        }

        return Math.abs(area) / 2;
    }
}