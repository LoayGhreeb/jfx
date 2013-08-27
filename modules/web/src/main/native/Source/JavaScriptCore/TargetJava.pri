PRECOMPILED_HEADER = $$PWD/javascriptcorejava_pch.h

CONFIG += staticlib depend_includepath precompile_header

VPATH += \
    $$PWD \
    $$PWD/pcre \

INCLUDEPATH += \
    $$GENERATED_SOURCES_DIR \
    $$PWD \
    $$PWD/.. \
    $$PWD/assembler \
    $$PWD/bytecode \
    $$PWD/bytecompiler \
    $$PWD/heap \
    $$PWD/dfg \
    $$PWD/debugger \
    $$PWD/disassembler \
    $$PWD/interpreter \
    $$PWD/jit \
    $$PWD/llint \
    $$PWD/parser \
    $$PWD/profiler \
    $$PWD/runtime \
    $$PWD/tools \
    $$SOURCE_DIR/WTF \
    $$SOURCE_DIR/WTF/wtf \
    $$SOURCE_DIR/WTF/gobject \
    $$SOURCE_DIR/WTF/unicode \
    $$SOURCE_DIR/WTF/java \
    $$PWD/yarr \
    $$PWD/API \
    $$PWD/ForwardingHeaders \
    ../../../generated-src/headers

include(yarr/yarr.pri)
include($$PWD/../WTF/WTFJava.pri)

INSTALLDEPS += all

SOURCES += \
    API/JSBase.cpp \
    API/JSCallbackConstructor.cpp \
    API/JSCallbackFunction.cpp \
    API/JSCallbackObject.cpp \
    API/JSClassRef.cpp \
    API/JSContextRef.cpp \
    API/JSObjectRef.cpp \
    API/JSScriptRef.cpp \
    API/JSStringRef.cpp \
#   API/JSStringRefQt.cpp \
    API/JSValueRef.cpp \
    API/JSWeakObjectMapRefPrivate.cpp \
    API/OpaqueJSString.cpp \
    assembler/ARMAssembler.cpp \
    assembler/ARMv7Assembler.cpp \
    assembler/LinkBuffer.cpp \
    assembler/MacroAssembler.cpp \
    assembler/MacroAssemblerARM.cpp \
    bytecode/ArrayAllocationProfile.cpp \
    bytecode/ArrayProfile.cpp \
    bytecode/CallLinkInfo.cpp \
    bytecode/CallLinkStatus.cpp \
    bytecode/CodeBlock.cpp \
    bytecode/CodeBlockHash.cpp \
    bytecode/CodeOrigin.cpp \
    bytecode/CodeType.cpp \
    bytecode/DFGExitProfile.cpp \
    bytecode/ExecutionCounter.cpp \
    bytecode/ExitKind.cpp \
    bytecode/GetByIdStatus.cpp \
    bytecode/JumpTable.cpp \
    bytecode/LazyOperandValueProfile.cpp \
    bytecode/MethodOfGettingAValueProfile.cpp \
    bytecode/Opcode.cpp \
    bytecode/PolymorphicPutByIdList.cpp \
    bytecode/PreciseJumpTargets.cpp \
    bytecode/PutByIdStatus.cpp \
    bytecode/ReduceWhitespace.cpp \
    bytecode/ResolveGlobalStatus.cpp \
    bytecode/SamplingTool.cpp \
    bytecode/SpecialPointer.cpp \
    bytecode/SpeculatedType.cpp \
    bytecode/StructureStubClearingWatchpoint.cpp \
    bytecode/StructureStubInfo.cpp \
    bytecode/UnlinkedCodeBlock.cpp \
    bytecode/Watchpoint.cpp \
    bytecompiler/BytecodeGenerator.cpp \
    bytecompiler/NodesCodegen.cpp \
    heap/CopiedSpace.cpp \
    heap/CopyVisitor.cpp \
    heap/ConservativeRoots.cpp \
    heap/DFGCodeBlocks.cpp \
    heap/Weak.cpp \
    heap/WeakBlock.cpp \
    heap/WeakHandleOwner.cpp \
    heap/WeakSet.cpp \
    heap/HandleSet.cpp \
    heap/HandleStack.cpp \
    heap/BlockAllocator.cpp \
    heap/GCThreadSharedData.cpp \
    heap/GCThread.cpp \
    heap/Heap.cpp \
    heap/HeapStatistics.cpp \
    heap/HeapTimer.cpp \
    heap/IncrementalSweeper.cpp \
    heap/JITStubRoutineSet.cpp \
    heap/MachineStackMarker.cpp \
    heap/MarkStack.cpp \
    heap/MarkedAllocator.cpp \
    heap/MarkedBlock.cpp \
    heap/MarkedSpace.cpp \
    heap/SlotVisitor.cpp \
    heap/SuperRegion.cpp \
    heap/VTableSpectrum.cpp \
    heap/WriteBarrierSupport.cpp \
    debugger/DebuggerActivation.cpp \
    debugger/DebuggerCallFrame.cpp \
    debugger/Debugger.cpp \
    dfg/DFGAbstractState.cpp \
    dfg/DFGArgumentsSimplificationPhase.cpp \
    dfg/DFGArrayMode.cpp \
    dfg/DFGAssemblyHelpers.cpp \
    dfg/DFGBackwardsPropagationPhase.cpp \
    dfg/DFGByteCodeParser.cpp \
    dfg/DFGCapabilities.cpp \
    dfg/DFGCommon.cpp \
    dfg/DFGCFAPhase.cpp \
    dfg/DFGCFGSimplificationPhase.cpp \
    dfg/DFGCPSRethreadingPhase.cpp \
    dfg/DFGConstantFoldingPhase.cpp \
    dfg/DFGCSEPhase.cpp \
    dfg/DFGDCEPhase.cpp \
    dfg/DFGDisassembler.cpp \
    dfg/DFGDominators.cpp \
    dfg/DFGDriver.cpp \
    dfg/DFGEdge.cpp \
    dfg/DFGFixupPhase.cpp \
    dfg/DFGGraph.cpp \
    dfg/DFGJITCompiler.cpp \
    dfg/DFGLongLivedState.cpp \
    dfg/DFGMinifiedNode.cpp \
    dfg/DFGNode.cpp \
    dfg/DFGNodeFlags.cpp \
    dfg/DFGOperations.cpp \
    dfg/DFGOSREntry.cpp \
    dfg/DFGOSRExit.cpp \
    dfg/DFGOSRExitCompiler.cpp \
    dfg/DFGOSRExitCompiler64.cpp \
    dfg/DFGOSRExitCompiler32_64.cpp \
    dfg/DFGOSRExitJumpPlaceholder.cpp \
    dfg/DFGPhase.cpp \
    dfg/DFGPredictionPropagationPhase.cpp \
    dfg/DFGPredictionInjectionPhase.cpp \
    dfg/DFGRepatch.cpp \
    dfg/DFGSpeculativeJIT.cpp \
    dfg/DFGSpeculativeJIT32_64.cpp \
    dfg/DFGSpeculativeJIT64.cpp \
    dfg/DFGTypeCheckHoistingPhase.cpp \
    dfg/DFGThunks.cpp \
    dfg/DFGUnificationPhase.cpp \
    dfg/DFGUseKind.cpp \
    dfg/DFGValueSource.cpp \
    dfg/DFGVariableAccessDataDump.cpp \
    dfg/DFGVariableEvent.cpp \
    dfg/DFGVariableEventStream.cpp \
    dfg/DFGValidate.cpp \
    dfg/DFGVirtualRegisterAllocationPhase.cpp \
    disassembler/Disassembler.cpp \
    interpreter/AbstractPC.cpp \
    interpreter/CallFrame.cpp \
    interpreter/Interpreter.cpp \
    interpreter/JSStack.cpp \
    jit/ClosureCallStubRoutine.cpp \
    jit/ExecutableAllocatorFixedVMPool.cpp \
    jit/ExecutableAllocator.cpp \
    jit/HostCallReturnValue.cpp \
    jit/GCAwareJITStubRoutine.cpp \
    jit/JITArithmetic.cpp \
    jit/JITArithmetic32_64.cpp \
    jit/JITCall.cpp \
    jit/JITCall32_64.cpp \
    jit/JITCode.cpp \
    jit/JIT.cpp \
    jit/JITDisassembler.cpp \
    jit/JITExceptions.cpp \
    jit/JITOpcodes.cpp \
    jit/JITOpcodes32_64.cpp \
    jit/JITPropertyAccess.cpp \
    jit/JITPropertyAccess32_64.cpp \
    jit/JITStubRoutine.cpp \
    jit/JITStubs.cpp \
    jit/JITThunks.cpp \
    jit/JumpReplacementWatchpoint.cpp \
    jit/ThunkGenerators.cpp \
    llint/LLIntCLoop.cpp \
    llint/LLIntData.cpp \
    llint/LLIntEntrypoints.cpp \
    llint/LLIntExceptions.cpp \
    llint/LLIntSlowPaths.cpp \
    llint/LLIntThunks.cpp \
    llint/LowLevelInterpreter.cpp \
    parser/Lexer.cpp \
    parser/Nodes.cpp \
    parser/ParserArena.cpp \
    parser/Parser.cpp \
    parser/SourceProvider.cpp \
    parser/SourceProviderCache.cpp \
    profiler/ProfilerBytecode.cpp \
    profiler/ProfilerBytecode.h \
    profiler/ProfilerBytecodeSequence.cpp \
    profiler/ProfilerBytecodes.cpp \
    profiler/ProfilerBytecodes.h \
    profiler/ProfilerCompilation.cpp \
    profiler/ProfilerCompilation.h \
    profiler/ProfilerCompilationKind.cpp \
    profiler/ProfilerCompilationKind.h \
    profiler/ProfilerCompiledBytecode.cpp \
    profiler/ProfilerCompiledBytecode.h \
    profiler/ProfilerDatabase.cpp \
    profiler/ProfilerDatabase.h \
    profiler/ProfilerExecutionCounter.h \
    profiler/ProfilerOrigin.cpp \
    profiler/ProfilerOrigin.h \
    profiler/ProfilerOriginStack.cpp \
    profiler/ProfilerOriginStack.h \
    profiler/ProfilerOSRExit.cpp \
    profiler/ProfilerOSRExitSite.cpp \
    profiler/ProfilerProfiledBytecodes.cpp \
    profiler/Profile.cpp \
    profiler/ProfileGenerator.cpp \
    profiler/ProfileNode.cpp \
    profiler/LegacyProfiler.cpp \
    runtime/ArgList.cpp \
    runtime/Arguments.cpp \
    runtime/ArrayConstructor.cpp \
    runtime/ArrayPrototype.cpp \
    runtime/BooleanConstructor.cpp \
    runtime/BooleanObject.cpp \
    runtime/BooleanPrototype.cpp \
    runtime/CallData.cpp \
    runtime/CodeCache.cpp \
    runtime/CodeSpecializationKind.cpp \
    runtime/CommonIdentifiers.cpp \
    runtime/Completion.cpp \
    runtime/ConstructData.cpp \
    runtime/DateConstructor.cpp \
    runtime/DateConversion.cpp \
    runtime/DateInstance.cpp \
    runtime/DatePrototype.cpp \
    runtime/ErrorConstructor.cpp \
    runtime/Error.cpp \
    runtime/ErrorInstance.cpp \
    runtime/ErrorPrototype.cpp \
    runtime/ExceptionHelpers.cpp \
    runtime/Executable.cpp \
    runtime/FunctionConstructor.cpp \
    runtime/FunctionExecutableDump.cpp \
    runtime/FunctionPrototype.cpp \
    runtime/GCActivityCallback.cpp \
    runtime/GetterSetter.cpp \
    runtime/Options.cpp \
    runtime/Identifier.cpp \
    runtime/IndexingType.cpp \
    runtime/InitializeThreading.cpp \
    runtime/InternalFunction.cpp \
    runtime/JSActivation.cpp \
    runtime/JSAPIValueWrapper.cpp \
    runtime/JSArray.cpp \
    runtime/JSCell.cpp \
    runtime/JSDateMath.cpp \
    runtime/JSFunction.cpp \
    runtime/JSBoundFunction.cpp \
    runtime/VM.cpp \
    runtime/JSGlobalObject.cpp \
    runtime/JSGlobalObjectFunctions.cpp \
    runtime/JSProxy.cpp \
    runtime/JSLock.cpp \
    runtime/JSNotAnObject.cpp \
    runtime/JSObject.cpp \
    runtime/JSONObject.cpp \
    runtime/JSPropertyNameIterator.cpp \
    runtime/JSSegmentedVariableObject.cpp \
    runtime/JSWithScope.cpp \
    runtime/JSNameScope.cpp \
    runtime/JSScope.cpp \
    runtime/JSString.cpp \
    runtime/JSStringJoiner.cpp \
    runtime/JSSymbolTableObject.cpp \
    runtime/JSCJSValue.cpp \
    runtime/JSVariableObject.cpp \
    runtime/JSWrapperObject.cpp \
    runtime/LiteralParser.cpp \
    runtime/Lookup.cpp \
    runtime/MathObject.cpp \
    runtime/MemoryStatistics.cpp \
    runtime/NameConstructor.cpp \
    runtime/NameInstance.cpp \
    runtime/NamePrototype.cpp \
    runtime/NativeErrorConstructor.cpp \
    runtime/NativeErrorPrototype.cpp \
    runtime/NumberConstructor.cpp \
    runtime/NumberObject.cpp \
    runtime/NumberPrototype.cpp \
    runtime/ObjectConstructor.cpp \
    runtime/ObjectPrototype.cpp \
    runtime/Operations.cpp \
    runtime/PropertyDescriptor.cpp \
    runtime/PropertyNameArray.cpp \
    runtime/PropertySlot.cpp \
    runtime/PropertyTable.cpp \
    runtime/PrototypeMap.cpp \
    runtime/RegExpConstructor.cpp \
    runtime/RegExpCachedResult.cpp \
    runtime/RegExpMatchesArray.cpp \
    runtime/RegExp.cpp \
    runtime/RegExpObject.cpp \
    runtime/RegExpPrototype.cpp \
    runtime/RegExpCache.cpp \
    runtime/SamplingCounter.cpp \
    runtime/SmallStrings.cpp \
    runtime/SparseArrayValueMap.cpp \
    runtime/StrictEvalActivation.cpp \
    runtime/StringConstructor.cpp \
    runtime/StringObject.cpp \
    runtime/StringPrototype.cpp \
    runtime/StringRecursionChecker.cpp \
    runtime/StructureChain.cpp \
    runtime/Structure.cpp \
    runtime/StructureRareData.cpp \
    runtime/SymbolTable.cpp \
    runtime/Watchdog.cpp \
    runtime/WatchdogNone.cpp \
    tools/CodeProfile.cpp \
    tools/CodeProfiling.cpp \
    yarr/YarrJIT.cpp \
    $$PWD/../WTF/wtf/java/MainThreadJava.cpp \
    $$PWD/../WTF/wtf/java/StringJava.cpp

contains(DEFINES, ICU_UNICODE=1) {
    SOURCES += $$PWD/../WTF/wtf/unicode/icu/CollatorICU.cpp
} else {
    SOURCES += $$PWD/../WTF/wtf/unicode/java/UnicodeJava.cpp
}

win32-* {
    procarch = $$(PROCESSOR_ARCHITECTURE) $$(PROCESSOR_ARCHITEW6432)
    contains(procarch, AMD64) {
        ASM_SOURCES += $$PWD/jit/JITStubsMSVC64.asm

        asm_compiler.CONFIG = target_predeps
        asm_compiler.dependency_type = TYPE_C
        asm_compiler.commands = ml64 /c /Fo ${QMAKE_FILE_OUT} ${QMAKE_FILE_IN}
        asm_compiler.output = ${QMAKE_VAR_OBJECTS_DIR}${QMAKE_FILE_BASE}$${first(QMAKE_EXT_OBJ)}
        asm_compiler.input = ASM_SOURCES
        asm_compiler.variable_out = OBJECTS
        asm_compiler.name = compiling[asm] ${QMAKE_FILE_IN}

        QMAKE_EXTRA_COMPILERS += asm_compiler
        DEFAULT_TARGETS += compiler_asm_compiler_make_all
    }
}