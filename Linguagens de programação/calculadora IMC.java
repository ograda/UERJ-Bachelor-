import java.io.*;
import java.util.*;
import java.math.*;
 
public class Main {
    public static void main(String args[]) throws IOException {
        BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
                char sexo;
        int n;
        String str;
        double d;
                Scanner scan = new Scanner(System.in);
                System.out.println("Digite o numero de pessoas: ");
                n = scan.nextInt();
                Mulher WMen = new Mulher();
                Mulher[] WMEN = new Mulher[n];
                Homem Men = new Homem();
                Homem[] MEN = new Homem[n];
                char[] ids = new char[n];
               
                for (int i=0; i<n; i++) {
                        sexo = scan.next().charAt(0);
                        while ((sexo != 'h') && (sexo != 'm')) {
                                System.out.println("--- Opcao Invalida!!!");
                                System.out.println("Inserir homem (h) ou mulher(m)? ");
                                sexo = scan.next().charAt(0);
                        } ids[i] = sexo;
                   
                        System.out.println("Digite o nome: ");
                        str = scan.next();
                        if (sexo=='h')
                                Men.setNome(str);
                        else
                                WMen.setNome(str);
                       
                        System.out.println("Digite a data de nascimento: ");
                        str = scan.next();
                        if (sexo=='h')
                                Men.setNasc(str);
                        else
                                WMen.setNasc(str);
                       
                        System.out.println("Digite o peso: ");
                        while (!scan.hasNextDouble()) {
                                        System.out.println("--- O peso deve ser um numero real!!!");
                                        scan.nextLine();
                                        } d = scan.nextDouble();
                                        if (sexo=='h')
                                Men.setPeso(d);
                        else
                                WMen.setPeso(d);
                                       
                        System.out.println("Digite a altura (em metros): ");
                        while (!scan.hasNextDouble()) {
                                        System.out.println("--- A altura deve ser um numero real!!!");
                                        scan.nextLine();
                                        } d = scan.nextDouble();
                        if (sexo=='h')
                                Men.setAltura(d);
                        else
                                WMen.setAltura(d);
                       
                        if (sexo=='h') {
                                Men.ToString();
                                MEN[i] = Men;
                        }
                        else {
                                WMen.ToString();
                                WMEN[i] = WMen;
                        }
                }
               
                for (int i=0; i<n; i++)
                        if (ids[i]=='h')
                                System.out.println(MEN[i].resultIMC());
                        else
                                System.out.println(WMEN[i].resultIMC());
 
        in.readLine();
    }
}
 
class Pessoa {
        private String nome, nascimento;
        public String getNome()  {
                return nome;
        }
        public void setNome(String a) {
                this.nome = a;
        }
        public String getNascimento()  {
                return nascimento;
        }
        public void setNasc(String a) {
                this.nascimento = a;
        }
}
 
class PessoaIMC extends Pessoa {
        private double peso, altura;
        private String str;
       
        public double getPeso() {
                return peso;
        }
        public void setPeso(double a) {
                this.peso = a;
        }
        public double getAltura()  {
                return altura;
        }
        public void setAltura(double a) {
                this.altura = a;
        }
        public double calculaIMC(double a, double b) {
                return (a/(b*b));
        }
       
        public String getInfo() {
                return str;
        }
       
        public void ToString() {
                this.str = "--------\nNome: " + getNome() + "\nData de Nascimento: " + getNascimento() + "\nPeso: " + getPeso() + "\nAltura: " + getAltura();
        }
}
 
class Homem extends PessoaIMC {
        public String resultIMC()  {
                String str, teste="";
                double imc = calculaIMC(getPeso(),getAltura());
                if (imc >= 26.4)
                        teste = "Acima do peso ideal";
                else
                        if (imc <= 20.7)
                                teste = "Abaixo do peso ideal";
                        else
                                teste = "Peso ideal";
                       
                str = getInfo() + "\nIMC: " + imc + " " + teste+ "\n--------";
                return str;
        }
}
 
class Mulher extends PessoaIMC {
        public String resultIMC()  {
                String str, teste="";
                double imc = calculaIMC(getPeso(),getAltura());
                if (imc >= 25.8)
                        teste = "Acima do peso ideal";
                else
                        if (imc <= 19)
                                teste = "Abaixo do peso ideal";
                        else
                                teste = "Peso ideal";
                       
                str = getInfo() + "\nIMC: " + imc + " " + teste + "\n--------";
                return str;
        }
}