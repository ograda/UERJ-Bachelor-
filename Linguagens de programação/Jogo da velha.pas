program prova09;
uses
crt;
 
var
i,j,opcao,vez,jogadas:integer;
casas:array[1..3,1..3] of string[1];
nome:array[1..2] of string[15];
fim:boolean;
 
procedure start;
var
i,j,c:integer;
begin
c:=0;
for i:=1 to 3 do
    for j:=1 to 3 do
    begin
    c:=c+1;
    str(c,casas[i][j]);
    end;
        vez:=0;
    jogadas:=0;
end;
 
procedure registro;
begin
textcolor(11);
write('Nome do jogador 1 = '); readln(nome[1]);
if (nome[1] = '') then nome[1] := 'Jogador 1';
 
textcolor(14);
write('Nome do jogador 2 = '); readln(nome[2]);
if (nome[2] = '') then nome[2] := 'Jogador 2';
if (nome[2] = nome[1]) then nome[2]:=nome[2]+' (2)';
end;
 
procedure inicial;
begin
clrscr;
textcolor(2);
for i:=1 to 9 do begin
gotoxy(30,4+i); writeln('     |     |     '); end;
for i:=1 to 2 do begin
gotoxy(30,4+(3*i)); writeln('_____ _____ _____'); end;
for i:=1 to 3 do
    for j:= 1 to 3 do
    begin
    gotoxy(26+(6*j),3+(3*i));
    textcolor(2);
    if (casas[i,j] = 'X') then textcolor(11);
    if (casas[i,j] = 'O') then textcolor(14);
    write(casas[i,j]);
    end;
gotoxy(1,17);
end;
 
function escolhe(jog:integer):integer;
var
x:integer;
begin
x:=10;
    while (x < 1) or (x > 9) do
    begin
         textcolor(8+(jog*3));
         write(nome[jog],', em qual casa gostaria de jogar ?  ');
        readln(x);
    end;
    escolhe:=x;
end;
 
procedure checaesc(jogador:integer);
var
check:boolean;
y:integer;
par:char;
begin
if jogador = 1 then par:='X' else par:='O'; check:=false;
    while check = false do
    begin
    y:=escolhe(jogador);
        case y of
           1:if casas[1,1] = '1' then begin casas[1,1]:=par; check:=true; end;
           2:if casas[1,2] = '2' then begin casas[1,2]:=par; check:=true; end;
           3:if casas[1,3] = '3' then begin casas[1,3]:=par; check:=true; end;
           4:if casas[2,1] = '4' then begin casas[2,1]:=par; check:=true; end;
           5:if casas[2,2] = '5' then begin casas[2,2]:=par; check:=true; end;
           6:if casas[2,3] = '6' then begin casas[2,3]:=par; check:=true; end;
           7:if casas[3,1] = '7' then begin casas[3,1]:=par; check:=true; end;
           8:if casas[3,2] = '8' then begin casas[3,2]:=par; check:=true; end;
           9:if casas[3,3] = '9' then begin casas[3,3]:=par; check:=true; end;
        else
           exit; {erro no programa, abortando}
        end;
    end;
end;
 
procedure fimdejogo(jogador:integer);
begin
jogadas:=jogadas+1;
    for i:=1 to 3 do
    if ((casas[i][1] = casas[i][2]) and (casas[i][2] = casas[i][3])) then
    begin
    fim:=true;
    write(nome[jogador],' VENCEU!')
    end;
    for j:=1 to 3 do
    if ((casas[1][j] = casas[2][j]) and (casas[2][j] = casas[3][j])) then
    begin
    fim:=true;
    write(nome[jogador],' VENCEU!')
    end;
    if ((casas[1,1] = casas[2,2]) and (casas[2,2] = casas[3,3])) or ((casas[1,3] = casas[2,2]) and (casas[2,2] = casas[3,1])) then
    begin
    fim:=true;
    write(nome[jogador],' VENCEU!')
    end;
    if jogadas = 9 then
       if fim = true then
       write(nome[jogador],' VENCEU!')
       else
       begin
       fim:=true;
       textcolor(2);
       writeln('deu velha!');
       end;
end;
 
 
begin
start;
registro;
inicial;
while fim <> true do
begin
        if vez < 2 then vez:=vez+1 else vez:=vez-1;
        checaesc(vez);
    inicial;
    fimdejogo(vez);
end;
readkey;
end.