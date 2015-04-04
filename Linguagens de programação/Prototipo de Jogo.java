package jogo;
 
import java.io.*;
import java.util.*;
import java.math.*;
 
public class Main {
        public static long tInicial = System.currentTimeMillis(), tFinal;  
       
    public static void main(String args[]) throws IOException {
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    boolean opt = true;
    MenorMaior MeMa = new MenorMaior();
    CaraCoroa CaCo = new CaraCoroa();
    MeMa.resetar();
    CaCo.resetar();
    Scanner scan = new Scanner(System.in);
        while (opt) opt = iniciar(scan, MeMa, CaCo);
        finalizar();
    }
   
    public static boolean iniciar(Scanner scan, MenorMaior MeMa, CaraCoroa CaCo) {
        int jId = 4;
        System.out.println("Opções:\n(0) Exit\n(1) Jogar MenorMaior \n(2) Jogar CaraCoroa\n(3) Resetar Scoreboards\nQual opção deseja selecionar? ");
           
            do {
                while (!scan.hasNextInt()) scan.nextLine();
                jId = scan.nextInt();
                if (jId < 0 || jId >= 4) System.out.println("Essa não é uma opção válida, tente novame: ");
            } while (jId < 0 || jId >= 4);
           
                if (jId == 0) return false;
                if (jId == 1) MeMa.jogar(scan);
                if (jId == 2) CaCo.jogar(scan);
                if (jId == 3) {
                MeMa.resetar();
                CaCo.resetar();
        }
 
        return true;
    }
   
    public static void finalizar() {
        double t;
        tFinal = System.currentTimeMillis();
        t = (double) tFinal - tInicial;
        int H = (int) Math.floor( ((t)/(60*60*1000)) ),M = (int) Math.floor( ((t)/(60*1000)) ),S = (int) Math.ceil( ((t)/(1000)) );
        M = M - 60*H;
        S = S - (3600*H + 60*M);
        System.out.println("Obrigado pela utilização do software!\nTempo de Jogo: " + H + " Horas " + M + " Minutos " + S + " Segundos.");
        System.exit(0);
    }
}
 
interface JogoInterface {
  public void resetar();
  public Placar jogar();
  public void abortar();
 
}
 
class Placar {
        private int vitorias, derrotas, jogos;
        public int getV()  {
                return vitorias;
        }
        public int getD()  {
                return derrotas;
        }
        public int getJogos()  {
                return jogos;
        }
        public void addV() {
                this.vitorias = vitorias+1;
                this.jogos = jogos+1;
        }
        public void addD() {
                this.derrotas = derrotas+1;
                this.jogos = jogos+1;
        }
 
        public void resetarScore() {
                this.vitorias = 0;
                this.derrotas = 0;
                this.jogos = 0;
        }
}
 
class CaraCoroa extends Placar implements JogoInterface {
    private int moeda, best, row;
    private Placar P;
    private boolean abort;
   
    public void newGame() {
        this.moeda = (int) (1 + (Math.random() * 2));
    }
   
    public boolean setBest(int t) {
        if (t > this.best) {
                this.best = t;
                return true;
        }
    return false;              
    }
   
    public void resetar() {
        resetarScore();
        this.best = 0;
        this.row = 0;
    }
   
    public void abortar() {
        this.abort = true;
        this.row = 0;
    }
   
    public Placar jogar(Scanner scan) {
        int r = -1;
        newGame();
        this.abort = false;
        System.out.println("O jogo Cara Coroa comecou... ");
        System.out.println("resposta =  " + this.moeda);
        while (r == -1 && (!this.abort)) {
                System.out.println("Escolha Cara (1) Ou Coroa (2) (outro número para abortar) : ");
                while (!scan.hasNextInt()) {
                System.out.println("insira (1) para cara ou (2) para coroa!\nOutro número para abortar.");
                scan.nextLine();
            } r = scan.nextInt();
            if (r != 1 && r != 2)
                abortar();
        }
       
        if (this.abort || this.moeda != r) {
                System.out.println("DERROTA!");
                addD();
                this.row = 0;
        } else {
                System.out.println("VITORIA!");
                addV();
                this.row++;
                this.abort = setBest(row);
                if (this.abort) System.out.println("PARABÉNS! Você acaba de fazer o melhor score!\n" + this.best + " Vitórias Consecutivas!!");
        }
        System.out.println("\nOverral Rating: " + getJogos() + " Jogos \n" + getV() + " V / " + getD() + " D (" + ((getV()*100)/getJogos()) + "%)\nMelhor Score: " + this.best + "  Vitórias Consecutivas!!\n");
 
        return P;
    }
 
        @Override
        public Placar jogar() {
                return null;
        }
}
 
class MenorMaior extends Placar implements JogoInterface {
        private int resposta, tryes, best;
        private Placar P;
        private boolean abort;
       
        public void newGame() {
            this.resposta = (1 + (int) (Math.random() * 100));
            this.tryes = 0;
        }
       
        public boolean setBest(int t) {
                if (getV() == 0) {
                        this.best = t;
                return true;
                }
                else
                if (t < this.best) {
                        this.best = t;
                        return true;
                }
        return false;                  
        }
       
        public void resetar() {
            resetarScore();
            this.best = 0;
        }
       
        public void abortar() {
            this.abort = true;
        }
       
        public Placar jogar(Scanner scan) {
                int r = -1;
                newGame();
                this.abort = false;
                System.out.println("O jogo Maior Menos comecou... ");
 
                while ((r != this.resposta) && (!this.abort)) {
                        System.out.println("Digite um número: ");
                        while (!scan.hasNextInt()) {
                        System.out.println("insira um inteiro para continuar");
                        this.tryes++;
                        scan.nextLine();
                    } r = scan.nextInt();
                    if (r != 0) {
                        this.tryes++;
                        if (r > this.resposta ) System.out.println("O numero fornecido eh MAIOR que o sorteado.");
                        if (r < this.resposta ) System.out.println("O numero fornecido eh MENOR que o sorteado.");
                    }
                    else
                        abortar();
                }
               
                if (this.abort) {
                        System.out.println("DERROTA!");
                        addD();
                } else {
                        System.out.println("VITORIA!");
                        this.abort = setBest(this.tryes);
                        addV();
                        if (this.abort) System.out.println("PARABÉNS! Você acaba de fazer o melhor score!");
                }
                System.out.println("Numero Sorteado:" + this.resposta + "\nTentativas: " + this.tryes + "\n\nOverral Rating: " + getJogos() + " Jogos \n" + getV() + " V / " + getD() + " D (" + ((getV()*100)/getJogos()) + "%)\nMelhor Score: " + this.best + " rodadas!\n");
 
            return P;
        }
 
                @Override
                public Placar jogar() {
                        return null;
                }
}