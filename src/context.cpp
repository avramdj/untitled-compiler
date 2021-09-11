//
// Created by avram on 30.8.21..
//
#include <iostream>
#include <exception>

#include <ast.hpp>
#include <context.h>

#include "llvm/IR/IRBuilder.h"
#include <llvm/IR/LegacyPassManager.h>

#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Utils.h"

#include "llvm/Target/TargetOptions.h"
#include "llvm/Target/TargetMachine.h"
#include <llvm/Support/Host.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/TargetRegistry.h>
#include "llvm/Support/FileSystem.h"

 #define _DISABLE_OPTS_ 1

using namespace llvm;

namespace backend {
    Module *TheModule;
    LLVMContext TheContext;
    IRBuilder<> Builder(TheContext);
    llvm::legacy::FunctionPassManager *TheFPM;
    Function *PrintfFja;
    Function *Sprintf;
    std::map<std::string, std::pair<AllocaInst *, Type *>> NamedValues;

    namespace Types {
        std::map<std::string, Type*> typeTable;
        std::map<std::string, std::map<std::string, int>> classVarTable;
        std::map<std::string, std::vector<std::string>> classFnTable;
    }

    void InitializeContext(void) {
        InitializeModuleAndPassManager();
        InitializeTypeTable();
    }

    void InitializeTypeTable(void) {
        Types::typeTable["int"] = Type::getInt32Ty(TheContext);
        Types::typeTable["bool"] = Type::getInt1Ty(TheContext);
        Types::typeTable["float"] = Type::getDoubleTy(TheContext);
        Types::typeTable["string"] = Type::getInt8PtrTy(TheContext);
    }

    void InitializeModuleAndPassManager(void) {
        TheModule = new Module("my_module", TheContext);

        /* printf fja */
        FunctionType *FT1 = FunctionType::get(IntegerType::getInt32Ty(TheContext),
                                              PointerType::get(Type::getInt8Ty(TheContext), 0), true);
        PrintfFja = Function::Create(FT1, Function::ExternalLinkage, "printf", TheModule);

        /* sprintf fja */
        FunctionType *FT2 = FunctionType::get(IntegerType::getInt32Ty(TheContext),
                                              PointerType::get(Type::getInt8Ty(TheContext), 0), true);
        Sprintf = Function::Create(FT2, Function::ExternalLinkage, "sprintf", TheModule);

        // Create a new pass manager attached to it.
        TheFPM = new llvm::legacy::FunctionPassManager(TheModule);

#ifndef _DISABLE_OPTS_
        // Do simple "peephole" optimizations and bit-twiddling optzns.
        TheFPM->add(createInstructionCombiningPass());
        // Reassociate expressions.
        TheFPM->add(createReassociatePass());
        // Eliminate Common SubExpressions.
        TheFPM->add(createNewGVNPass());
        // Simplify the control flow graph (deleting unreachable blocks, etc).
        TheFPM->add(createCFGSimplificationPass());
        TheFPM->add(createPromoteMemoryToRegisterPass());
#endif
        TheFPM->doInitialization();

    }

    void printModule(bool printIR, std::string outPath) {
        if(printIR) {
            TheModule->print(outs(), nullptr);
        }
        auto TargetTriple = sys::getDefaultTargetTriple();

        InitializeAllTargetInfos();
        InitializeAllTargets();
        InitializeAllTargetMCs();
        InitializeAllAsmParsers();
        InitializeAllAsmPrinters();

        std::string Error;
        auto Target = TargetRegistry::lookupTarget(TargetTriple, Error);

        if (!Target) {
            throw TargetErrorException(Error);
        }

        auto CPU = "generic";
        auto Features = "";

        TargetOptions opt;
        auto RM = Optional<Reloc::Model>();
        auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

        TheModule->setDataLayout(TargetMachine->createDataLayout());
        TheModule->setTargetTriple(TargetTriple);

        const auto& Filename = outPath;
        std::error_code EC;
        raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);

        if (EC) {
            std::cerr << "Could not open file: " << EC.message() << std::endl;
            throw FileException(EC.message());
        }

        legacy::PassManager pass;
        auto FileType = CGFT_ObjectFile;

        if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
            throw TargetErrorException("TargetMachine can't emit a file of this type");
        }

        pass.run(*TheModule);
        dest.flush();
        dest.close();

        delete TheModule;
    }

    Value *Types::getTypeConstant(Type * t, float val) {
        if(t == typeTable["int"]) {
            return ConstantInt::get(Type::getInt32Ty(TheContext), APInt(32, val));
        }
        if(t == typeTable["float"]) {
            return ConstantFP::get(Type::getDoubleTy(TheContext), val);
        }
        return nullptr;
    }
    Value *Types::getTypeConstant(std::string type, float val) {
        return getTypeConstant(typeTable[type], val);
    }
    Type *Types::getType(std::string name) {
        auto typeIt = Types::typeTable.find(name);
        if(typeIt == Types::typeTable.end()) {
            std::cerr << "Invalid type  " << name << std::endl;
            return nullptr;
        }
        return typeIt->second;
    }
    Value *Types::boolCast(Value *v) {
        Type *boolType = Types::getType("bool");
        Type *vT = v->getType();
        if(vT == Types::getType("float")) {
            return Builder.CreateFPToUI(v, boolType, "boolcast");
        } else if(vT == Types::getType("int")) {
            return Builder.CreateIntCast(v, boolType, false, "boolcast");
        } else if (vT == Types::getType("bool")) {
            return v;
        } else {
            return nullptr;
        }
    }
    Value *Types::floatCast(Value *v) {
        Type *floatType = Types::getType("float");
        Type *vT = v->getType();
        if(vT == Types::getType("float")) {
            return v;
        } else if(vT == Types::getType("int")) {
            return Builder.CreateSIToFP(v, floatType, "boolcast");
        } else {
            return nullptr;
        }
    }
}

