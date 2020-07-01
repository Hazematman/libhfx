project('libhfx')
fs = import('fs')
mips_compiler = find_program('mips64-elf-gcc')
mips_linker = find_program('mips64-elf-ld')
mips_objcopy = find_program('mips64-elf-objcopy')
n64tool = find_program('n64tool')
chksum64 = find_program('chksum64')
bash = find_program('bash')

#####################################
# Add new files here
n64_tool_path = '/home/lucas.fryzek/.local/' # TODO replace this with not hardcoded value
src_dir = './libhfx_src/'
src_files = [
    'main.c',
]

ucode_src_dir = './ucode_src/'
ucode_files = [
    'main.s',
    'hfx_cmd_int.s',
    'hfx_cmd_nop.s',
    'hfx_cmd_dma_read.s',
]
ucode_loader = 'libhfx_src/linker.s'

# Change compiler flags here
cflags = [
'-I'+meson.source_root()+'/include',
'-std=gnu99',
'-march=r4000',
'-mtune=r4000',
'-O0',
'-Wall',
'-MMD',
]

ldflags = [
'-L'+n64_tool_path+'mips64-elf/lib',
'-ldragon',
'-lc',
'-lm',
'-ldragonsys',
'-Tn64.ld',
]

ucode_cflags = [
'-I'+meson.source_root()+'/include',
'-std=gnu99',
'-march=r4000',
'-mtune=r4000',
'-O0',
'-Wall',
'-x',
'assembler-with-cpp',
'-MMD',
]

#####################################

src = []
foreach source : src_files
    src += [src_dir + source]
endforeach

ucode_src_files = []
foreach source : ucode_files
    src_file = ucode_src_dir + source
    ucode_src_files += [src_file]
endforeach

ucode_define_string = '-DTEXT_SECTION_BIN="@0@" -DDATA_SECTION_BIN="@1@"'

# Need to replace $BUILD_DIR in linker script so project can build from any directory
ucode_linker_script = custom_target('ucode_linker_script',
    output : 'ucode.ld',
    input : 'ucode_template.ld',
    command : [bash, '-c', 'export BUILD_DIR=ucode_elf@cus; envsubst < @INPUT@'], # hardcode ucode_elf@cus for now
    capture : true )

ucode_obj_generator = generator(mips_compiler,
    output  : '@BASENAME@.o',
    arguments : ucode_cflags +['-c', '@INPUT@', '-o', '@OUTPUT@'],
    depfile : '@BASENAME@.d')

# Build all ucode object files
ucode_obj_files = ucode_obj_generator.process(ucode_src_files)

# Build ucode into elf file
ucode_elf_file = custom_target('ucode_elf',
   output : 'ucode.elf',
   input : ucode_obj_files,
   command : [mips_linker, '@INPUT@', '-o', '@OUTPUT@', '-T', ucode_linker_script])
   
# convert ucode elf into bin files
ucode_bin = custom_target('ucode_bin',
    output : ['ucode_text.bin', 'ucode_data.bin'],
    input : ucode_elf_file,
    command : [mips_objcopy, '--dump-section', '.text=ucode_text.bin', 
               '--dump-section', '.data=ucode_data.bin', '@INPUT@'])
               
# Build linker object file
ucode_link = custom_target('ucode_link',
    output : '@BASENAME@.o',
    input : ucode_loader,
    depfile : '@BASENAME@.d',
    depends: ucode_bin,
    command : [mips_compiler] + ucode_cflags + 
              ucode_define_string.format(ucode_bin[0].full_path(), ucode_bin[1].full_path()).split() +
              ['-c', '@INPUT@', '-o', '@OUTPUT@'])
              
# Build all c source files
object_generator = generator(mips_compiler,
    output : '@BASENAME@.o',
    arguments : cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'],
    depfile : '@BASENAME@.d')
object_files = object_generator.process(src)

# TODO change this so we build library and tests are real applications
# Build final elf file
elf_file = custom_target('elf',
    output : 'prog.elf',
    input : [object_files] + [ucode_link],
    command : [mips_linker] + ['-o', '@OUTPUT@', '@INPUT@'] + ldflags)
    
bin_file = custom_target('bin',
    output : 'prog.bin',
    input : elf_file,
    command : [mips_objcopy, '@INPUT@', '@OUTPUT@', '-O', 'binary'])
    
unfixed_binary = custom_target('unfixed_binary',
    output : 'prog.z64',
    input: bin_file,
    command : [n64tool, '-l', '2M', '-h', n64_tool_path+'/mips64-elf/lib/header', '-o', '@OUTPUT@', '@INPUT@'])

#run_target('fixed_binary', command : [chksum64, unfixed_binary])

fixed_binary = custom_target('fixed_binary',
    output : 'prog2.z64', # TODO find out if this is okay 
    input : unfixed_binary,
    command : [chksum64, '@INPUT@'], build_by_default:true)
    
