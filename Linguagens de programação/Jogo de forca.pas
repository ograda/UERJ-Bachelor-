program prova10;
uses crt;
 
var
palavra:string[12];
letras:array[1..26] of string[1];
erradas:array[1..7] of string[1];
remain,escolhidas:integer;
fim:boolean;
 
procedure escolher;
begin
write('escolha uma palavra (12 letras no maximo)  ');
readln(palavra);
remain:=7;
escolhidas:=0;
fim:=false;
end;
 
procedure jogo;
var
i,j,k,l:integer;
begin
clrscr;
gotoxy(5,6);
for k:=1 to 26 do
    for l:=1 to length(palavra) do
        if upcase(palavra[l]) = letras[k] then
        begin
        gotoxy(1+(5*l),6); write(upcase(palavra[l]));
        end;
gotoxy(5,7);
for i:=1 to length(palavra) do
write('___  ');
gotoxy(5,10);
write('letras erradas:  ');
for j:=1 to 7 do
write(erradas[j],'  ');
gotoxy(5,11);
write('tentativas restantes:  ',remain);
end;
 
procedure fimdojogo;
var
l,k,lets:integer;
begin
jogo;
lets:=0;
         for l:=1 to length(palavra) do
         begin
            for k:=1 to 26 do
            begin
                 if upcase(palavra[l]) = letras[k] then
                 begin
                 lets:=lets+1;
                 end;
            end;
          end;
     if lets = length(palavra) then
        begin
          writeln;
          writeln;
          writeln;
          writeln('PARABENS! voce venceu!');
          fim:=true;
         end;
 
end;
 
procedure checar(let:char);
var
i,j:integer;
corr:boolean;
begin
corr:=false;
    for j:=1 to 26 do
        if letras[j] = let then
         begin
          corr:=true;
          write('     Essa letra ja foi escolhida!');
          delay(300);
         end;
 
    if corr=false then
       for i:=1 to length(palavra) do
       begin
       if corr = true then
       break;
           if upcase(palavra[i]) = let then
           begin
           corr:=true;
           escolhidas:=escolhidas+1;
           letras[escolhidas]:=let;
           fimdojogo;
           end;
        end;
 
    if corr=false then
    begin
    escolhidas:=escolhidas+1;
    erradas[8-remain]:=let;
    letras[escolhidas]:=let;
    remain:=remain-1;
          if remain = 0 then
          begin
          clrscr;
          gotoxy(15,15);
          write('VOCE PERDEU! a palavra era:  ',palavra);
          fim:=true;
          end;
    end;
end;
 
procedure jogar;
var
maiuscula,letra:char;
begin
gotoxy(5,15);
write('Em que letra gostaria de jogar?  '); readln(letra);
maiuscula:=upcase(letra);
while (maiuscula < chr(65)) or (maiuscula > chr(90)) do
begin write('     Digite uma LETRA!  '); readln(letra);  maiuscula:=upcase(letra); end;
checar(maiuscula);
end;
 
begin
escolher;
while fim <> true do
begin
jogo;
jogar;
end;
readkey;
end.