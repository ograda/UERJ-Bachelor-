import java.io.*;
import java.util.*;
import java.math.*;
 
public class Main {
    public static void main(String args[]) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
        int base, destino;
        int n;
        String init, out;
                LinkedList Lista = new LinkedList(), Descarte = new LinkedList();
                Scanner scan = new Scanner(System.in);
                System.out.println("Digite o tipo/base de origem (1) binário (2) decimal (3) octal (4) hex: ");
                base = scan.nextInt();
                System.out.println("Digite o tipo/base de destino (1) binário (2) decimal (3) octal (4) hex: ");
                destino = scan.nextInt();
                init = scan.next();
               
                n = decimalBase(init, base);
                out = "";
                switch (destino) {
                    case 1:
                        out = "binário";
                        init = decToBin(n);
                    break;
                    case 2:
                        out = "decimal";
                        init = String.valueOf(n);
                    break;
                    case 3:
                        out = "octal";
                        init = decToOct(n);
                    break;
                    case 4:
                        out = "hex";
                        init = decToHex(n);
                    break;
                    default:
                        System.out.println("Esta não é uma conversão válida!");
                        System.exit(0);
                }
 
                System.out.println("O valor em " + out + " é: " + init);
 
        in.readLine();
    }
   
    public static int decimalBase(String a, int kind) {
                        switch (kind) {
                    case 1:
                        return Integer.parseInt(a, 2);
                    case 2:
                        return Integer.parseInt(a);
                    case 3:
                        return Integer.parseInt(a, 8);
                    case 4:
                        return Integer.parseInt(a, 16);
                    default:
                        System.out.println("Esta não é uma conversão válida!");
                        System.exit(0);
                }
        return 0;
    }
   
    public static String decToBin(int a) {
                return Integer.toBinaryString(a);
    }
   
    public static String decToOct(int a) {
                return Integer.toOctalString(a);
    }
   
    public static String decToHex(int a) {
                return Integer.toHexString(a);
    }
}