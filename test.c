#include "dpi.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "lua.h"
#include "lauxlib.h" 

static dpiContext *gContext = NULL;

int showError(void) 
{
    dpiErrorInfo info;

    dpiContext_getError(gContext, &info);
    fprintf(stderr, "ERROR: %.*s (%s: %s)\n", info.messageLength, info.message,
            info.fnName, info.action);
    return -1;
}

static void fatalError(const char *message)
{
    fprintf(stderr, "FATAL: %s\n", message);
    exit(1);
}

static void finalize(void)
{
    dpiContext_destroy(gContext);
}


static void getParams(void)
{
    dpiErrorInfo errorInfo;

    if (!gContext) {
        if (dpiContext_create(DPI_MAJOR_VERSION, DPI_MINOR_VERSION, &gContext, &errorInfo) < 0) {
            fprintf(stderr, "ERROR: %.*s (%s : %s)\n", errorInfo.messageLength,
                    errorInfo.message, errorInfo.fnName, errorInfo.action);
            fatalError("Cannot create DPI context.");
        }
        atexit(finalize);
    }

}



dpiConn *getConn(dpiCommonCreateParams *commonParams) 
{
  dpiConn *conn;
  //dpiPool *pool;
  getParams();
  if (dpiConn_create(gContext, "onm", 3, "onm",3, "onyma11", 7, commonParams, NULL, &conn) < 0) {
    showError();
    fatalError("Unable to create connect");
  }
  return conn;
}



int test_dpi()
{
  dpiConn *conn;
  dpiStmt *stmt;
  char sql[] = "select user from dual";
  uint32_t numCol; 
  int found;
  uint32_t bufferRowIndex;
  dpiData *data;
  dpiNativeTypeNum nativeTypeNum;

  conn = getConn(NULL);

  if (dpiConn_prepareStmt(conn, 0, sql, strlen(sql), NULL, 0, &stmt)) {
    return showError();
  }
  if (dpiStmt_execute(stmt, 0, &numCol) < 0) {
    return showError();
  }
  if (dpiStmt_fetch(stmt, &found, &bufferRowIndex) < 0 ) {
      return showError();
  }
  dpiStmt_getQueryValue(stmt, 3, &nativeTypeNum, &data);
  dpiStmt_release(stmt);
  fprintf(stderr, "QQQ %s", "qqq");
  return 0;
}



static int icube(lua_State *L){                
	float rtrn = lua_tonumber(L, -1);      
	printf("Top of cube(), number=%f\n",rtrn);
	lua_pushnumber(L,rtrn*rtrn*rtrn);      
	return 1;                              
}

LUA_API int luaopen_test (lua_State *L) {
	lua_register(L,"cube",icube);

	return 0;
}
