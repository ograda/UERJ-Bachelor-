#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <iostream>
#include <string.h>

using namespace std;

bool xmlClear(char *xmlDoc) {
    xmlDocPtr doc;
    xmlNodePtr cur, tmp;
    bool found = false;
    doc = xmlParseFile(xmlDoc);
    if (doc == NULL) { cout << "Failed to parse " << xmlDoc << ".. exiting" << endl; exit(1); }

    cur = xmlDocGetRootElement(doc);
    xmlSetProp(cur, (const xmlChar *) "queue", (const xmlChar *) "0");
    cur = cur->xmlChildrenNode;

    while (cur != NULL && strcmp (((char*) cur->name),"Games")) cur = cur->next;
    if (!strcmp (((char*) cur->name),"Games")) {
        cur = cur->xmlChildrenNode;
        if (cur != NULL)
            tmp = cur->next;
            while (cur != NULL) {
                xmlUnlinkNode(cur);
                xmlFreeNode(cur);
                cur = tmp;
                if (cur != NULL)
                    tmp = cur->next;

            }
    }
    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;

    while (cur != NULL && strcmp (((char*) cur->name),"Queue")) cur = cur->next;
    if (!strcmp (((char*) cur->name),"Queue")) {
        cur = cur->xmlChildrenNode;
        if (cur != NULL)
        tmp = cur->next;
            while (cur != NULL) {
                xmlUnlinkNode(cur);
                xmlFreeNode(cur);
                cur = tmp;
                if (cur != NULL)
                    tmp = cur->next;
            }

    }
    xmlSaveFile(xmlDoc,doc);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
    return found;
}

bool checkNodeChield(char *xmlDoc, char *node, char *id, bool removeChild) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    bool found = false;
    doc = xmlParseFile(xmlDoc);
    if (doc == NULL) { cout << "Failed to parse " << xmlDoc << ".. exiting" << endl; exit(1); }

    cur = xmlDocGetRootElement(doc);
    cur = cur->xmlChildrenNode;
    while (cur != NULL) {
        if (!strcmp (((char*) cur->name),node)) {
            cur = cur->xmlChildrenNode;
            while (cur != NULL) {
                if (!strcmp (((char*) cur->name),id)) {
                    if (removeChild) {
                        xmlUnlinkNode(cur);
                        xmlFreeNode(cur);
                        xmlSaveFile(xmlDoc,doc);
                    }
                    found = true;
                    break;
                }
                cur = cur->next;
            }
            break;
        }
        cur = cur->next;
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
    return found;
}


xmlChar *checkXML(char *xmlDoc, char *node, char *atributte, bool father) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlChar *ret = ((xmlChar *) "ERROR");
    doc = xmlParseFile(xmlDoc);

    if (doc == NULL) { cout << "Failed to parse " << xmlDoc << ".. exiting" << endl; exit(1); }

    cur = xmlDocGetRootElement(doc);
    if (father) ret =  xmlGetProp(cur,  (const xmlChar *) atributte);
    else {
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
            if (!strcmp (((char*) cur->name),node))
               ret = xmlGetProp(cur, (const xmlChar *) atributte);
            cur = cur->next;
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
    if (ret == NULL) {cout << "Failed to parse " << xmlDoc << " node '" << node << "' atributte '" << atributte << "' (existe?).. exiting" << endl; exit(1); }
    return ret;
}

void xmlWrite(char *xmlDoc, char *node, char *atributte, bool atualizar, char *value) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    doc = xmlParseFile(xmlDoc);

    if (doc == NULL) { cout << "Failed to parse " << xmlDoc << ".. exiting" << endl; exit(1); }

    cur = xmlDocGetRootElement(doc);
    if (!strcmp (((char*) cur->name),node)) xmlSetProp(cur, (const xmlChar *) atributte, (const xmlChar *) value);
    else {
        cur = cur->xmlChildrenNode;
        while (cur != NULL) {
            if (!strcmp (((char*) cur->name),node))
                if (atualizar)
                    xmlSetProp(cur, (const xmlChar *) atributte, (const xmlChar *) value);
                else
                     xmlNewChild(cur, NULL, BAD_CAST value, NULL);
            cur = cur->next;
        }
    }
    xmlSaveFile(xmlDoc,doc);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    xmlMemoryDump();
}

int main(int argc, char *argv[]) {
    bool procura;
    xmlChar *ate;

    // pega um valor no xml, node, filho -- o bool serve para checar se e o rootnode.
    ate = checkXML("jogo.xml", "gameIds", "p1",false);
    cout << "teste ate == " << ate << endl;

    // atualiza um valor de um atributo de um node;
    xmlWrite("jogo.xml", "Jogo", "resp", true, "110");

    // insere um filho em um node;
    xmlWrite("jogo.xml", "queue", "p1", false, "filho");

    // procura um filho em um node -- o bool determina se apos encontrar o filho, este sera deletado
    procura = checkNodeChield("jogo.xml", "queue", "filho", true);
    cout << "procura 1 = " << procura << endl;

    // procura um filho em um node -- o bool determina se apos encontrar o filho, este sera deletado
    procura = checkNodeChield("jogo.xml", "queue", "naoAchou", false);
    cout << "procura 2 = " << procura << endl;

    return 0;
}


