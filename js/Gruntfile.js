module.exports = function(grunt) {
  grunt.initConfig({
    pkg: grunt.file.readJSON('package.json'),

    clean: {
      build:{
        src: ['build/']
      }
    },

    jshint: {
      files: ['Gruntfile.js', 'src/**/*.js', 'test/**/*.js'],
      options: {
        globals: {
          devel: true
        }
      }
    },

    karma: {
      unit: {
        configFile: 'karma.conf.js'
      }
    },

    uglify: {
      options: {
        banner: '/*! <%= pkg.name %> <%= pkg.version %> <%= grunt.template.today("yyyy-mm-dd") %> */\n',
        global_defs: {
          'DEBUG': false
        }
      },
      dist: {
        files: [{
          expand: true,
          cwd: 'src/',
          src: '*.js',
          dest: 'build/'
        }]
      }
    },

    watch: {
      files: ['<%= jshint.files %>'],
      tasks: ['jshint']
    }
  });

  //grunt.loadTasks('tasks');

  // grunt plugins
  grunt.loadNpmTasks('grunt-contrib-clean');
  grunt.loadNpmTasks('grunt-contrib-jshint');
  grunt.loadNpmTasks('grunt-contrib-uglify');
  grunt.loadNpmTasks('grunt-contrib-watch');
  grunt.loadNpmTasks('grunt-karma');

  // tasks
  grunt.registerTask('build', ['clean','uglify']);
  grunt.registerTask('lint', ['jshint']);
  grunt.registerTask('test', ['karma']);
  grunt.registerTask('default', ['lint','test','build']);
};
