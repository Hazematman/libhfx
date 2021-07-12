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
    'basic_rb_test' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'basic_rdp_test' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'rdp_buffer_wrap' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'hfx_clear' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'hfx_draw_single_tri' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'hfx_cube' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'hfx_tex_tri' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'hfx_persp' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'hfx_persp_tex' : {
        'src' : ['main.c'],
        'ucode_src' : [],
    },
    'ucode_tri' : {
        'src' : ['main.c'],
        'ucode_src' : ['ucode.c', 'ucode_asm.S'],
    },
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
'-g',
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

ucode_cflags = [
'-I'+prj_root+'/include',
'-I'+prj_root+'/ucode_src/include',
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
'-I'+prj_root+'/include',
'-I'+prj_root+'/ucode_src/include',
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

ucode_linker_script = prj_root+'/ucode.ld'
ucode_crt = prj_root+'/ucode_src/src/crt.S'
ucode_loader = meson.source_root()+'/loader.S'

#####################################

# Build all c source files
object_generator = generator(mips_compiler,
    output : '@BASENAME@.o',
    arguments : cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'],
    depfile : '@BASENAME@.d')

ucode_object_generator = generator(mips_compiler,
    output : '@BASENAME@.o',
    arguments : ucode_cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'],
    depfile : '@BASENAME@.d')

foreach test, test_sources : test_dict
    source_files = []
    foreach source : test_sources['src']
        file = meson.source_root() + '/' + test + '/' + source
        source_files += [file]
    endforeach

    ucode_deps = []

    if 0 != test_sources['ucode_src'].length()
        ucode_source_files = [ucode_crt]
        foreach source : test_sources['ucode_src']
            file = meson.source_root() + '/' + test + '/' + source
            ucode_source_files += [file]
        endforeach

        ucode_object_files = ucode_object_generator.process(ucode_source_files)

        # Build ucode into elf file
        ucode_elf_file = custom_target(test+'_ucode_elf',
           output : test+'.ucode.elf',
           input : ucode_object_files,
           depend_files : ucode_linker_script,
           command : [mips_compiler] + ucode_ldflags + ['@INPUT@', '-o', '@OUTPUT@', '-T', ucode_linker_script])

        # convert ucode elf into bin files
        ucode_bin = custom_target(test+'_ucode_bin',
            output : [test+'_ucode_text.bin', test+'_ucode_data.bin'],
            input : ucode_elf_file,
            command : [mips_objcopy, '--dump-section', '.text='+test+'_ucode_text.bin', 
                       '--dump-section', '.data='+test+'_ucode_data.bin', '@INPUT@'])

        ucode_loader = custom_target(test+'_ucode_loader',
            output : '@BASENAME@.o',
            input : ucode_loader,
            depfile : '@BASENAME@.d',
            depends : ucode_bin,
            command : [mips_compiler] + cflags + ['-c', '@INPUT@', '-o', '@OUTPUT@'])

        ucode_deps += [ucode_loader]
    endif
    
    object_files = object_generator.process(source_files)

    # Build final elf file
    elf_file = custom_target(test+'_elf',
        output : test+'.elf',
        input : [object_files] + ucode_deps,
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
