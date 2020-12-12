project('tests')
fs = import('fs')

# Need to get exe extension to properly run on windows
exe_extension = ''
if build_machine.system().contains('mingw')
    exe_extension = '.exe'
endif

mips_compiler = find_program('mips64-elf-gcc' + exe_extension)
mips_linker = find_program('mips64-elf-ld' + exe_extension)
mips_objcopy = find_program('mips64-elf-objcopy' + exe_extension)
n64tool = find_program('n64tool' + exe_extension)
chksum64 = find_program('chksum64' + exe_extension)

prj_root = meson.source_root() + '/..'

#####################################
# Add new tests & files here
n64_tool_path = run_command('sh' + exe_extension, '-c', 'echo $N64_INST').stdout().strip()

test_dict = {
    'basic_rb_test' : [
        'main.c'
    ],
    'basic_rdp_test' : [
        'main.c'
    ],
    'rdp_buffer_wrap' : [
        'main.c'
    ],
    'hfx_clear' : [
        'main.c'
    ],
    'hfx_draw_single_tri' : [
        'main.c'
    ],
    'hfx_cube' : [
        'main.c'
    ],
    'hfx_tex_tri' : [
        'main.c'
    ],
}

# Change compiler flags here
cflags = [
'-I'+prj_root+'/include',
'-I'+prj_root+'/libhfx_src/include',
'-std=gnu99',
'-march=vr4300',
'-mtune=vr4300',
'-O0',
'-Wall',
'-MMD',
]

hfx_lib_path = prj_root+'/build'
hfx_lib_name = hfx_lib_path + '/libhfx.a'

ldflags = [
'-L'+n64_tool_path+'/mips64-elf/lib',
'-L'+hfx_lib_path,
'-L'+meson.source_root()+'/build',
'--start-group',
'-lhfx',
'-ldragon',
'-lc',
'-lm',
'-ldragonsys',
'--end-group',
'-Tn64.ld',
]

#####################################

# Build all c source files
object_generator = generator(mips_compiler,
    output : '@BASENAME@.o',
    arguments : cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'],
    depfile : '@BASENAME@.d')

foreach test, sources : test_dict
    source_files = []
    foreach source : sources
        file = meson.source_root() + '/' + test + '/' + source
        source_files += [file]
    endforeach
    
    object_files = object_generator.process(source_files)

    # Build final elf file
    elf_file = custom_target(test+'_elf',
        output : test+'.elf',
        input : [object_files],
        depend_files : hfx_lib_name,
        command : [mips_linker, '-o', '@OUTPUT@', '@INPUT@'] + ldflags)
        
    bin_file = custom_target(test+'_bin',
        output : test+'.bin',
        input : elf_file,
        command : [mips_objcopy, '@INPUT@', '@OUTPUT@', '-O', 'binary'])
        
    unfixed_binary = custom_target(test+'_unfixed_binary',
        output : test+'.z64',
        input: bin_file,
        command : [n64tool, '-l', '2M', '-h', n64_tool_path+'/mips64-elf/lib/header', '-o', '@OUTPUT@', '@INPUT@'])

    fixed_binary = custom_target(test+'_fixed_binary',
        output : test+'2.z64', # TODO find out if this is okay 
        input : unfixed_binary,
        command : [chksum64, '@INPUT@'], build_by_default:true)
endforeach