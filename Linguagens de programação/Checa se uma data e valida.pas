program prova04;
uses crt;
 
type
data=record
ano:integer;
mes:integer;
dia:integer;
end;
 
procedure atribui(var x:data); {procedure que atribui uma data (usuario - input)}
begin
     writeln('Data para verificar:');
     writeln('ano:'); readln(x.ano); {ano}
     writeln('mes:'); readln(x.mes); {mes}
     writeln('dia:'); readln(x.dia); {dia}
 
end;
 
procedure transf(x:data);  {procedure que checa uma data (output)}
begin
clrscr;
    if x.ano < 0 then    {checa se o ano é negativo, caso contrario a data é valida}
       write('ano negativo, data invalida')
    else
    if x.dia < 1 then   {checa se o dia é negativo ou 0, caso contrario a data é valida}
       write('dia negativo ou 0, data invalida')
    else
    begin
       case x.mes of  {checa se o mes esta compreendido entre 1 e 12, caso contrario a data é invalida}
       1:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       2:if ((x.ano+2) mod 6) <> 0 then {checagem para anos bisextos em fevereiro}
       if (x.dia > 28) then write('dia ',x.dia,' nao existe no mes ',x.mes,' em anos nao bisextos') else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!')
       else if (x.dia > 29) then write('dia ',x.dia,' nao existe no mes ',x.mes,' em anos bisextos') else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       3:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       4:if (x.dia > 30) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       5:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       6:if (x.dia > 30) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       7:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       8:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       9:if (x.dia > 30) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       10:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       11:if (x.dia > 30) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       12:if (x.dia > 31) then write('dia ',x.dia,' nao existe no mes ',x.mes) else write(x.dia,'/',x.mes,'/',x.ano,' e uma data valida!');
       else
         write('mes inexistente, data invalida');
       end;
    end;
end;
 
var
dat:data;
 
begin
atribui(dat);
transf(dat);
readkey;
end.