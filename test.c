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

static int save_pushstring_wrapped(struct lua_State *L)
{
	char *str = (char *)lua_topointer(L, 1);
	lua_pushstring(L, str);
	return 1;
}

static int safe_pushstring(struct lua_State *L, char *str)
{
	lua_pushcfunction(L, save_pushstring_wrapped);
	lua_pushlightuserdata(L, str);
	return lua_pcall(L, 1, 1, 0);
}

/*
static int lua_push_error(struct lua_State *L)
{
	lua_pushnumber(L, -3);
	lua_insert(L, -2);
	return 2;
}
*/
static int lua_odpi_push_error(struct lua_State *L)
{
    dpiErrorInfo info;
    dpiContext_getError(gContext, &info);
    fprintf(stderr, "ERROR: %.*s (%s: %s)\n", info.messageLength, info.message, info.fnName, info.action);
	lua_pushnumber(L, -1);
	safe_pushstring(L, (char *)info.message);
	return 2;
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



dpiConn *getConn(lua_State *L, const char *username, const char *password, const char *dbname) 
{
    dpiCommonCreateParams *commonParams = NULL;
  dpiConn *conn;
  getParams();
  if (dpiConn_create(gContext, username, strlen(username), password,strlen(password), dbname, strlen(dbname), commonParams, NULL, &conn) < 0) {
      showError();
      fatalError("Unable to create connect");
    //return lua_odpi_push_error(L);
  }
  return conn;
}



int test_dpi(lua_State *L)
{
  dpiConn *conn;
  dpiStmt *stmt;
  char sql[] = "select user from dual";
  uint32_t numCol; 
  int found;
  uint32_t bufferRowIndex;
  dpiData *outputValue;
  dpiNativeTypeNum nativeTypeNum;

  conn = getConn(L, "onm", "onm", "onyma11");

  if (dpiConn_prepareStmt(conn, 0, sql, strlen(sql), NULL, 0, &stmt)) {
    return lua_odpi_push_error(L);
  }
  
  if (dpiStmt_execute(stmt, 0, &numCol) < 0) {
    return lua_odpi_push_error(L);
  }
  if (dpiStmt_fetch(stmt, &found, &bufferRowIndex) < 0 ) {
      return lua_odpi_push_error(L);
  }
  if (dpiStmt_getQueryValue(stmt, 1, &nativeTypeNum, &outputValue) < 0) {
      return lua_odpi_push_error(L);
  }

  lua_pushlstring(L, outputValue->value.asBytes.ptr, outputValue->value.asBytes.length);
  
  dpiStmt_release(stmt);
  dpiConn_release(conn);
  //fprintf(stderr, "QQQ %s", "qqq");
  return 1;
}

int lua_ora_connect(lua_State *L) {
	const char *user = lua_tostring(L, 1);
	const char *pass = lua_tostring(L, 2);
	const char *db = lua_tostring(L, 3);
    dpiCommonCreateParams *commonParams = NULL;
    dpiConn *conn;
    
    printf("Trying %s/%s@%s\n", user, pass, db);

    getParams();
    if (dpiConn_create(gContext, user, strlen(user), pass,strlen(pass), db, strlen(db), commonParams, NULL, &conn) < 0) {
        return lua_odpi_push_error(L);
    }
    
    //dpiConn *conn = getConn(L, user, pass, db);
    lua_pushnumber(L, 1);
    dpiConn **conn_p = (dpiConn **)lua_newuserdata(L, sizeof(dpiConn *));
	*conn_p = conn;
	//luaL_getmetatable(L, mysql_driver_label);
	//lua_setmetatable(L, -2);
    return 1;
}



static int icube(lua_State *L){                
	float rtrn = lua_tonumber(L, -1);      
	printf("Top of cube(), number=%f\n",rtrn);
	lua_pushnumber(L,rtrn*rtrn*rtrn);      
	return 1;                              
}


LUA_API int luaopen_test (lua_State *L) {
	lua_register(L,"cube",icube);
    lua_register(L,"dpi",test_dpi);
    lua_register(L, "ora_conn", lua_ora_connect);

	return 0;
}
