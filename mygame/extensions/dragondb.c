#include "dragonruby.h"
#include "sqlite/sqlite3.h"

drb_api_t *drb_api;

sqlite3 *db;


static mrb_value drdb_sqlite3_open(mrb_state *mrb, mrb_value self) {
  char *filename;
  mrb_get_args(mrb, "z", &filename);

  int rc = sqlite3_open(filename, &db);
  if (rc) {
    //fprintf(stderr, "Can't open '%s': %s\n", filename, sqlite3_errmsg(db));
    sqlite3_close(db);
    return mrb_nil_value();
  }
  return mrb_true_value();
}

static mrb_value drdb_sqlite3_close(mrb_state *mrb, mrb_value self) {
  int rc = sqlite3_close(db);
  if (rc) {
    return mrb_nil_value();
  }
  return mrb_true_value();
}

static mrb_value drdb_sqlite3_exec(mrb_state *mrb, mrb_value self) {
  char *sql;
  mrb_get_args(mrb, "z", &sql);

  char *zErrMsg = 0;
  int rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return mrb_nil_value();
  }
  return mrb_true_value();
}

// function to run a query that returns a result set
static mrb_value drdb_sqlite3_query(mrb_state *mrb, mrb_value self) {
  char *sql;
  mrb_get_args(mrb, "z", &sql);

  sqlite3_stmt *stmt;
  int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
  if (rc != SQLITE_OK) {
    return mrb_nil_value();
  }

  int cols = sqlite3_column_count(stmt);
  mrb_value result = mrb_ary_new(mrb);
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    mrb_value row = mrb_ary_new(mrb);
    for (int i = 0; i < cols; i++) {
      switch (sqlite3_column_type(stmt, i)) {
        case SQLITE_INTEGER:
          mrb_ary_push(mrb, row, mrb_fixnum_value(sqlite3_column_int(stmt, i)));
          break;
        case SQLITE_FLOAT:
          mrb_ary_push(mrb, row, mrb_float_value(mrb, sqlite3_column_double(stmt, i)));
          break;
        case SQLITE_TEXT:
          mrb_ary_push(mrb, row, mrb_str_new_cstr(mrb, (const char *)sqlite3_column_text(stmt, i)));
          break;
        case SQLITE_BLOB:
          mrb_ary_push(mrb, row, mrb_str_new_cstr(mrb, (const char *)sqlite3_column_blob(stmt, i)));
          break;
        case SQLITE_NULL:
          mrb_ary_push(mrb, row, mrb_nil_value());
          break;
      }
    }
    mrb_ary_push(mrb, result, row);
  }
  sqlite3_finalize(stmt);
  return result;
}

DRB_FFI_EXPORT
void drb_register_c_extensions_with_api(mrb_state *mrb, struct drb_api_t *api) {
  drb_api = api;
  struct RClass *FFI = drb_api->mrb_module_get(mrb, "GTK");
  struct RClass *module = drb_api->mrb_define_module_under(mrb, FFI, "DragonDB");

  // Define the module functions
  drb_api->mrb_define_module_function(mrb, module, "sqlite_open", drdb_sqlite3_open, MRB_ARGS_REQ(1));
  drb_api->mrb_define_module_function(mrb, module, "sqlite_close", drdb_sqlite3_close, MRB_ARGS_NONE());
  drb_api->mrb_define_module_function(mrb, module, "sqlite_exec", drdb_sqlite3_exec, MRB_ARGS_REQ(1));
  drb_api->mrb_define_module_function(mrb, module, "sqlite_query", drdb_sqlite3_query, MRB_ARGS_REQ(1));
}
