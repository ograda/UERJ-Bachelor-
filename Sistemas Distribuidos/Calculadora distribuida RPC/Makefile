rpcgen -C Geo.x
g++ -o ServerGeo Geo.cpp Geo_svc.c Geo_xdr.c -lrpcsvc

rpcgen -C Math.x
g++ -o ServerEspec Math.cpp Math_svc.c Math_xdr.c -lrpcsvc

rpcgen -C SCient.x
g++ -o ServerCient SCient.cpp SCient_svc.c SCient_xdr.c -lrpcsvc

rpcgen -C Euclid.x
g++ -o ServerEuclid Euclid.cpp Euclid_svc.c Euclid_xdr.c -lrpcsvc

rpcgen -C Global.x
g++ -o ServerGlobal Global.cpp Global_svc.c Global_xdr.c Geo_clnt.c Geo_xdr.c Math_clnt.c Math_xdr.c SCient_clnt.c SCient_xdr.c Euclid_clnt.c Euclid_xdr.c -lrpcsvc

g++ -o Client Client.cpp Global_clnt.c Global_xdr.c
