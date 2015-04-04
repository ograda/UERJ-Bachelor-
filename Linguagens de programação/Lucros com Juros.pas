program prova01;
uses crt;
 
type
arr=array[1..4,1..12] of real; {variavel para as procedures}
 
procedure atribui(var x:arr); {procedure que atribui os lucros (usuario - input)}
var
i,j:integer;
begin
     for i:=1 to 12 do
     begin
     Writeln(i,' mes:');
         for j:=1 to 4 do
         begin
         Writeln('digite os lucros da semana ',j);
         readln(x[j][i]); {pode ser usado da mesma forma que x[j,i]}
         end;
     end;
 
end;
 
procedure semanas(x:arr); {output dos lucros semanais}
var
i,j:integer;
begin
     for i:=1 to 12 do
     begin
     Writeln(i,' mes:');
         for j:=1 to 4 do
         begin
         Write('semana ',j);
         Writeln(' = ',x[j][i]:0:2);
         end;
     Writeln;
     end;
writeln('pressione uma tecla para mostrar o lucro em cada mes');
readkey;
end;
 
procedure meses(x:arr); {output dos lucros mensais}
var
i,j:integer;
soma:real;
begin
     for i:=1 to 12 do
     begin
     soma:=0;
     Write(i,' mes');
         for j:=1 to 4 do
         begin
         soma:=soma+x[j][i]
         end;
     Writeln(' = ',soma:0:2);
     end;
writeln;
writeln('pressione uma tecla para mostrar o lucro anual');
readkey;
end;
 
procedure anual(x:arr); {output do lucro anual}
var
i,j:integer;
soma:real;
begin
soma:=0;
     for i:=1 to 12 do
         for j:=1 to 4 do
         begin
         soma:=soma+x[j][i]
         end;
Writeln('o lucro anual foi ',soma:0:2);
readkey;
end;
 
var
vendas:array[1..4,1..12] of real;
begin
atribui(vendas);
clrscr;      {a tela foi zerada por mera estetica.. em todos os seguintes}
writeln('pressione uma tecla para mostrar o lucro em cada semana');
readkey;
clrscr;
semanas(vendas);
clrscr;
meses(vendas);
clrscr;
anual(vendas);
readkey;
end.