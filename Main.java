import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = Integer.parseInt(sc.nextLine());
        Map<String, String[]> gates = new HashMap<>();

        for (int i = 0; i < n; i++) {
            String[] parts = sc.nextLine().split("=");
            String gateName = parts[0].trim();
            String[] gateDefinition = parts[1].trim().split("[(), ]+");
            gates.put(gateName, gateDefinition);
        }

        int t = Integer.parseInt(sc.nextLine());
        Map<String, int[]> values = new HashMap<>();

        while (sc.hasNextLine()) {
            String line = sc.nextLine().trim();
            if (line.isEmpty()) continue; // Skip empty lines

            if (line.matches("[a-zA-Z]+")) {
                String targetGate = line.trim();
                simulateCircuit(gates, values, targetGate, t);
                break;
            }

            String[] parts = line.split(" ");
            String inputVar = parts[0];
            int[] timings = Arrays.stream(parts, 1, parts.length)
                    .filter(s -> !s.trim().isEmpty()) // Ignore empty strings
                    .mapToInt(Integer::parseInt)
                    .toArray();
            values.put(inputVar, timings);
        }

        sc.close();
    }

    private static void simulateCircuit(Map<String, String[]> gates, Map<String, int[]> values, String targetGate, int t) {
        Map<String, int[]> outputs = new HashMap<>();
        for (String gate : gates.keySet()) {
            outputs.put(gate, new int[t]);
        }

        for (int cycle = 1; cycle < t; cycle++) {
            for (String gate : gates.keySet()) {
                String[] definition = gates.get(gate);
                String operation = definition[0];
                String input1 = definition[1];
                String input2 = definition[2];

                int value1 = getValue(input1, outputs, values, cycle - 1);
                int value2 = getValue(input2, outputs, values, cycle - 1);

                int result = computeGate(operation, value1, value2);
                outputs.get(gate)[cycle] = result;
            }
        }

        int[] targetOutput = outputs.get(targetGate);
        for (int i = 0; i < t; i++) {
            System.out.print(targetOutput[i] + (i == t - 1 ? "\n" : " "));
        }
    }

    private static int getValue(String var, Map<String, int[]> outputs, Map<String, int[]> values, int cycle) {
        if (values.containsKey(var)) {
            return values.get(var)[cycle];
        } else {
            return outputs.get(var)[cycle];
        }
    }

    private static int computeGate(String operation, int value1, int value2) {
        switch (operation) {
            case "AND":
                return value1 & value2;
            case "OR":
                return value1 | value2;
            case "NAND":
                return ~(value1 & value2) & 1;
            case "NOR":
                return ~(value1 | value2) & 1;
            case "XOR":
                return value1 ^ value2;
            default:
                return 0;
        }
    }
}
