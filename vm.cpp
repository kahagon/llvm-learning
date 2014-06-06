#include <iostream>
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/LinkAllPasses.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/PrettyStackTrace.h"
#include "llvm/Support/Signals.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/ValueSymbolTable.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

Function* append_test_block_return_int(
  LLVMContext& ctx,
  IRBuilder<>* builder, 
  Module* mod, 
  Function* func) 
{
	BasicBlock *basic_block = BasicBlock::Create(ctx, "ret_int", func);
	builder->SetInsertPoint(basic_block);

	// TODO: generate function body
	int var_int_value = 34;
	std::string var_name = "i";
  Value* var = builder->CreateAlloca(Type::getInt32Ty(ctx),	0, var_name);
	ValueSymbolTable& function_value_symbol_table 
		= func->getValueSymbolTable();

	Value* lhs_v;
	Value* rhs_v;

	// 'lhs_v' is equal to 'var'
  lhs_v = function_value_symbol_table.lookup(var_name);
	rhs_v = ConstantInt::get(
    Type::getInt32Ty(ctx), var_int_value);
	builder->CreateStore(rhs_v, lhs_v);

	// return i
	builder->CreateRet(builder->CreateLoad(lhs_v, "var_tmp"));
	return func;
}

Function* append_test_block_print_string(
  LLVMContext& ctx,
  IRBuilder<>* builder,
  Module* mod,
  Function* func) 
{
  BasicBlock *block = BasicBlock::Create(ctx, "print_string", func);
  builder->SetInsertPoint(block);

	ValueSymbolTable& symbol_table = func->getValueSymbolTable();
  StringRef str_ref("Hello, World\n");
  Constant* string = ConstantDataArray::getString(ctx, str_ref);
}

Function *generate_function_echo(LLVMContext& ctx, IRBuilder<>* builder, Module *mod) {
	std::vector<Type*> param_types;
  param_types.push_back(Type::getInt64Ty(ctx));
	Type* result_type = Type::getVoidTy(ctx);
	std::string func_name = "echo";
	FunctionType* func_type 
		= FunctionType::get(result_type, param_types, false);
	Function* func = Function::Create(
		func_type, Function::ExternalLinkage,	func_name, mod);

  BasicBlock *block = BasicBlock::Create(ctx, "echo_string", func);
  builder->SetInsertPoint(block);

	ValueSymbolTable& symbol_table = func->getValueSymbolTable();
  StringRef str_ref("Hello, World\n");
  Constant* string = ConstantDataArray::getString(ctx, str_ref);


	return func;
}

Function *generate_function_main(LLVMContext& ctx, IRBuilder<>* builder, Module *mod) {
	std::vector<Type*> param_types(0, Type::getVoidTy(ctx));
	Type* result_type = Type::getInt32Ty(ctx);
	std::string func_name = "main";
	FunctionType* func_type 
		= FunctionType::get(result_type, param_types, false);
	Function* func = Function::Create(
		func_type, Function::ExternalLinkage,	func_name, mod);

	return func;
}

void ibm_tutorial(LLVMContext& ctx, IRBuilder<>* builder, Module *mod) {
  FunctionType* func_type = FunctionType::get(Type::getInt32Ty(ctx), false);
  Function* func = Function::Create(func_type, Function::ExternalLinkage, "main", mod);

  BasicBlock* entry = BasicBlock::Create(ctx, "entry", func);
  builder->SetInsertPoint(entry);

  Value* helloWorld = builder->CreateGlobalStringPtr("hello world!\n");

  std::vector<Type*> puts_args;
  puts_args.push_back(Type::getInt8Ty(ctx)->getPointerTo());
  ArrayRef<Type*> args_ref(puts_args);
  FunctionType* puts_type = FunctionType::get(Type::getInt32Ty(ctx), args_ref, false);
  Constant* puts_func = mod->getOrInsertFunction("puts", puts_type);

  builder->CreateCall(puts_func, helloWorld);

  
  // struct test_struct { int; char*; }; 
  std::vector<Type *> types;
  types.push_back(Type::getInt64Ty(ctx)); // integer member
  types.push_back(Type::getInt8Ty(ctx)->getPointerTo()); // member for string 
  StructType *st = StructType::get(ctx, types);

  UndefValue* value_test_struct = UndefValue::get(st);

  // register struct to global. create instance of GlobalVariable with new
  GlobalVariable *gb = new GlobalVariable(*mod, st, false, 
    llvm::GlobalValue::ExternalLinkage, value_test_struct, "test_struct");

  //builder->CreateRetVoid();
  Value* ret = ConstantInt::get(Type::getInt32Ty(ctx), 34);
  builder->CreateRet(ret);
}

void print_module(Module* mod, std::string file_name) {
	PassManager pass_manager;
	std::string error;
  //sys::fs::OpenFlags flags = sys::fs::OpenFlags::F_RW;
	raw_fd_ostream raw_stream(file_name.c_str(), error, sys::fs::F_RW);

	pass_manager.add(createPromoteMemoryToRegisterPass());
	pass_manager.add(createPrintModulePass(raw_stream));
	pass_manager.run(*mod);
	raw_stream.close();
}

/*
 * int main() {
 *     int i = 0;
 *     return i;
 * }
 */
int main(int argc, char *argv[]) {
	std::string file_name = "main.ll";
  LLVMContext& ctx = getGlobalContext();
	IRBuilder<> *builder = new IRBuilder<>(ctx);
	Module *mod = new Module("llvm-test", ctx);
	//Function* func = generate_function_main(ctx, builder, mod);

	ibm_tutorial(ctx, builder, mod);
  //mod->dump();

  //append_test_block_print_string(ctx, builder, mod, func);
  //append_test_block_return_int(ctx, builder, mod, func);

	print_module(mod, file_name);

	return 0;
}
