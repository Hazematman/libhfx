project('libhfx', 'c')
fs = import('fs')

# Need to get exe extension to properly run on windows
exe_extension = ''
if build_machine.system().contains('mingw')
    exe_extension = '.exe'
endif

mips_compiler = find_program('mips64-elf-gcc' + exe_extension)
mips_linker = find_program('mips64-elf-ld' + exe_extension)
mips_ar = find_program('mips64-elf-ar' + exe_extension)
mips_objcopy = find_program('mips64-elf-objcopy' + exe_extension)
n64tool = find_program('n64tool' + exe_extension)
chksum64 = find_program('chksum64' + exe_extension)
bash = find_program('bash' + exe_extension)

#####################################
# Add new files here
src_dir = './libhfx_src/'
src_files = [
    'src/hfx.c',
    'src/hfx_rb.c',
    'src/hfx_cmd_dma.c',
    'src/hfx_cmd.c',
    'src/hfx_display.c',
    'src/hfx_math.c',
    'src/hfx_render.c',
    'src/hfx_render_gl.c',
    'src/hfx_matrix.c',
    'src/hfx_state.c',
    'src/hfx_texture.c',
    'src/hfx_dispatch.c',
]

ucode_src_dir = './ucode_src/'
ucode_files = [
    'crt.S',
    'hfx_ucode.c',
]
ucode_loader = 'libhfx_src/src/linker.S'
ucode_linker_script = 'ucode.ld'

# Change compiler flags here
cflags = [
'-I'+meson.source_root()+'/include',
'-I'+meson.source_root()+'/libhfx_src/include',
'-std=gnu99',
'-march=vr4300',
'-mtune=vr4300',
'-O0',
'-Wall',
'-Wpedantic',
'-MMD',
'-g',
]

ucode_cflags = [
'-I'+meson.source_root()+'/include',
'-std=gnu99',
'-Wall',
'-march=vr4300',
'-mtune=vr4300',
'-Os',
'-mno-branch-likely',
'-mno-llsc',
'-Wall',
'-mabi=eabi',
'-mgp32',
'-flto',
'-MMD',
'-EB',
]

ucode_ldflags = [
'-Wall',
'-march=vr4300',
'-mtune=vr4300',
'-mabi=eabi',
'-mgp32',
'-ffreestanding',
'-nostdlib',
'-Os',
'-mno-branch-likely',
'-mno-llsc',
'-flto',
'-EB',
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

# Build all ucode object files
ucode_obj_files = []
foreach source : ucode_src_files
    obj = custom_target(fs.name(source), 
                        output : '@BASENAME@.o',
                        input : source,
                        command : [mips_compiler] + ucode_cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'],
                        depfile : '@BASENAME@.d')
    ucode_obj_files += [obj]
endforeach

# Build ucode into elf file
ucode_elf_file = custom_target('ucode_elf',
   output : 'ucode.elf',
   input : ucode_obj_files,
   depend_files : ucode_linker_script,
   command : [mips_compiler] + ucode_ldflags + ['@INPUT@', '-o', '@OUTPUT@', '-T', '../'+ucode_linker_script])
   
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

#############################################################

opengl_src = [
    './libhfx_gl/src/hfx.c',
    './libhfx_src/src/hfx_matrix.c',
    './gl_test/cube/main.c'
]

opengl_c_args = [
    '-I'+meson.source_root()+'/libhfx_gl/include',
    '-I'+meson.source_root()+'/include'
]


# Build OpenGL example platform
# current disable OpenGL example platform
'''
executable('cube', opengl_src,
            c_args : opengl_c_args,
            link_args : ['-lSDL2', '-lGL', '-lGLEW', '-lm'])
'''