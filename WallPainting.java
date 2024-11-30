import java.util.*;

public class WallPainting {

    // Function to calculate area of polygon using Shoelace formula
    public static double polygonArea(int[][] points, int N) {
        double area = 0;
        for (int i = 0; i < N; i++) {
            int j = (i + 1) % N;  // next vertex index (wrapping around)
            area += points[i][0] * points[j][1];
            area -= points[j][0] * points[i][1];
        }
        return Math.abs(area) / 2.0;
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        // Read the number of vertices
        int N = scanner.nextInt();

        // Array to store the vertices
        int[][] vertices = new int[N][2];

        // Read the coordinates of the vertices
        for (int i = 0; i < N; i++) {
            vertices[i][0] = scanner.nextInt();
            vertices[i][1] = scanner.nextInt();
        }

        // Read the brush size
        int M = scanner.nextInt();

        // Calculate the area of the polygon using Shoelace formula
        double area = polygonArea(vertices, N);

        // Calculate the number of MxM blocks needed to cover the area
        // Each press of MxM block will cover M * M area
        int blocks = (int) Math.ceil(area / (M * M));

        // Output the result
        System.out.println(blocks);

        scanner.close();
    }
}
