project('tests')
fs = import('fs')
mips_compiler = find_program('mips64-elf-gcc')
mips_linker = find_program('mips64-elf-ld')
mips_objcopy = find_program('mips64-elf-objcopy')
n64tool = find_program('n64tool')
chksum64 = find_program('chksum64')

prj_root = meson.source_root() + '/..'

#####################################
# Add new tests & files here
n64_tool_path = '/home/lucas.fryzek/.local/n64_toolchain/' # TODO replace this with not hardcoded value

test_dict = {
    'basic_rb_test' : [
        'main.c'
    ],
    'basic_rdp_test' : [
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

ldflags = [
'-L'+n64_tool_path+'mips64-elf/lib',
'-L'+prj_root+'/build',
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
        depend_files : prj_root+'/build/libhfx.a',
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