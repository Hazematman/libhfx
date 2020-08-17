project('libhfx')
fs = import('fs')
mips_compiler = find_program('mips64-elf-gcc')
mips_linker = find_program('mips64-elf-ld')
mips_ar = find_program('mips64-elf-ar')
mips_objcopy = find_program('mips64-elf-objcopy')
n64tool = find_program('n64tool')
chksum64 = find_program('chksum64')
bash = find_program('bash')

#####################################
# Add new files here
src_dir = './libhfx_src/'
src_files = [
    'src/hfx.c',
    'src/hfx_rb.c',
    'src/hfx_cmd_dma.c',
    'src/hfx_cmd.c',
    'src/hfx_display.c',
]

ucode_src_dir = './ucode_c_src/'
ucode_files = [
    'crt.S',
    'hfx_ucode.c',
]
ucode_loader = 'libhfx_src/src/linker.S'

# Change compiler flags here
cflags = [
'-I'+meson.source_root()+'/include',
'-I'+meson.source_root()+'/libhfx_src/include',
'-std=gnu99',
'-march=vr4300',
'-mtune=vr4300',
'-O0',
'-Wall',
'-MMD',
]

ucode_cflags = [
'-I'+meson.source_root()+'/include',
'-std=gnu99',
'-Wall',
'-march=vr4300',
'-mtune=vr4300',
'-Os',
'-mno-branch-likely',
'-Wall',
'-mabi=eabi',
'-mgp32',
'-flto',
'-MMD',
]

ucode_ldflags = [
'-Wall',
'-march=vr4300',
'-mtune=vr4300',
'-mabi=eabi',
'-mgp32',
'-ffreestanding',
'-nostdlib',
'-flto',
'-Os',
'-mno-branch-likely',
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
    command : [bash, '-c', 'export BUILD_DIR=ucode.elf.p; envsubst < @INPUT@'], # hardcode ucode_elf@cus for now
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
   command : [mips_compiler] + ucode_ldflags + ['@INPUT@', '-o', '@OUTPUT@', '-T', ucode_linker_script])
   
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
    command : [mips_compiler] + cflags + 
              ucode_define_string.format(ucode_bin[0].full_path(), ucode_bin[1].full_path()).split() +
              ['-c', '@INPUT@', '-o', '@OUTPUT@'])
              
# Build all c source files
object_generator = generator(mips_compiler,
    output : '@BASENAME@.o',
    arguments : cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'],
    depfile : '@BASENAME@.d')
object_files = object_generator.process(src)

# Build library file
libhfx = custom_target('libhfx',
    output : 'libhfx.a',
    input : [object_files] + [ucode_link],
    command : [mips_ar, 'rcs', '@OUTPUT@', '@INPUT@'],
    build_by_default:true)