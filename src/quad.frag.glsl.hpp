static const char *g_shader_fragment_quad = ""
#if defined(USE_LD)
"quad.frag.glsl"
#else
"#version 430\n"
"layout(location=0)uniform sampler2D surface_texture;"
"layout(location=1)uniform sampler3D noise_volume;"
"layout(location=2)uniform vec3[10] uniform_array;"
"mat3 noise_matrix=mat3(.42,-.7,.58,.53,.71,.46,-.74,.12,.67);"
"out vec4 output_color;"
"vec4 march_params=vec4(.3,.0,.7,.009);"
"vec3 light_direction=normalize(vec3(2.,.5,-1.));"
"float destruction=uniform_array[9].g;"
"bool world_toggle=1.<abs(uniform_array[7].b);"
"float sdf(vec3 point)"
"{"
"float aa,bb,cc,dd;"
"vec3 hh,ii,jj,kk;"
"if(world_toggle)"
"{"
"hh=point*.02;"
"vec4 rr=vec4(hh,1.);"
"for(int iter=0;"
"iter<9;"
"++iter)"
"{"
"rr.rgb=clamp(rr.rgb,-1.,1.)*2.-rr.rgb;"
"cc=dot(rr.rgb,rr.rgb);"
"rr*=clamp(max(.824/cc,.824),.0,1.);"
"rr=rr*vec4(vec3(-2.742),2.742)+vec4(hh,1.);"
"}"
"rr.rgb*=clamp(hh.g+1.,.1,1.);"
"cc=((length(rr.rgb)-3.259)/rr.a-.001475)*50.;"
"}"
"else"
"{"
"hh=noise_matrix*point*.007;"
"ii=noise_matrix*hh*2.61;"
"jj=noise_matrix*ii*2.11;"
"kk=noise_matrix*jj*2.11;"
"aa=texture(surface_texture,hh.rb).r*2.61;"
"bb=texture(surface_texture,ii.rb).r*1.77;"
"cc=texture(surface_texture,jj.rb).r*.11;"
"dd=texture(surface_texture,kk.rb).r*.11;"
"aa=dd+cc+pow(aa,2.)+pow(bb,2.);"
"cc=length(point.rb)*.3;"
"cc=aa*(smoothstep(.0,.5,cc*.0025)+.5)+point.g-6.*((sin(clamp(pow(cc/10.,1.8)-3.14/2.,-1.57,1.57))-1.)*2.+5.)*cos(clamp(.04*cc,.0,3.14));"
"}"
"hh=point-uniform_array[8].rgb;"
"aa=length(hh);"
"if(aa<destruction)return cc+destruction-aa;"
"return cc;"
"}"
"float raycast(vec3 point,vec3 direction,float interval,out vec3 out_point,out vec3 out_normal)"
"{"
"vec3 next,mid_point;"
"float prev_value=sdf(point),current_value,mid_value,ii=1.;"
"for(;"
"ii>.0;"
"ii-=interval)"
"{"
"next=point+direction*max(prev_value*march_params.b,.02);"
"current_value=sdf(next);"
"if(.0>current_value)"
"{"
"for(int jj=0;"
"jj<5;"
"++jj)"
"{"
"mid_point=(point+next)*.5;"
"mid_value=sdf(mid_point);"
"if(.0>mid_value)"
"{"
"next=mid_point;"
"current_value=mid_value;"
"}"
"else"
"{"
"point=mid_point;"
"prev_value=mid_value;"
"}"
"}"
"out_normal=normalize(vec3(sdf(next.rgb+march_params.rgg).r,sdf(next.rgb+march_params.grg).r,sdf(next.rgb+march_params.ggr).r)-current_value);"
"break;"
"}"
"point=next;"
"prev_value=current_value;"
"}"
"out_point=point;"
"return ii;"
"}"
"float C(inout vec3 p,vec3 d,vec3 c,float r)"
"{"
"vec3 q=p-c;"
"float e=dot(q,q)-r*r,a=dot(q,d);"
"if(0>e||0>a)"
"{"
"e=a*a-e;"
"if(0<e)"
"{"
"p+=max(-a-sqrt(e),.0)*d;"
"return length(a*d-q);"
"}"
"}"
"return .0;"
"}"
"vec3 Q(vec3 p)"
"{"
"vec3 a,b,c,h,i,j;"
"h=noise_matrix*p;"
"i=noise_matrix*h*3.;"
"j=noise_matrix*i*3.;"
"a=(texture(noise_volume,h).rgb-.5)*2.*.6;"
"b=(texture(noise_volume,i).rgb-.5)*2.*.3;"
"c=(texture(noise_volume,j).rgb-.5)*2.*.1;"
"return normalize(a+b+c);"
"}"
"void main()"
"{"
"vec2 c=gl_FragCoord.xy/360.-vec2(1.78,1.);"
"vec3 p=mix(mix(uniform_array[0],uniform_array[1],uniform_array[7].g),mix(uniform_array[1],uniform_array[2],uniform_array[7].g),uniform_array[7].g)*3.,d=normalize(mix(uniform_array[3],uniform_array[4],uniform_array[7].g)),q=mix(uniform_array[5],uniform_array[6],uniform_array[7].g),r=normalize(cross(d,q)),N,P;"
"q=normalize(cross(r,d));"
"d=normalize(d+c.r*r+c.g*q);"
"q=vec3(0);"
"float e,n;"
"r=vec3(109.,14.,86.);"
"if(0<int(uniform_array[7].b)%2&&.0<C(p,d,r,9.))"
"{"
"d=normalize(d+reflect(-d,normalize(p-r))*.2);"
"destruction=-.2;"
"world_toggle=!world_toggle;"
"}"
"if(world_toggle)march_params=vec4(.05,.0,.98,.022);"
"n=raycast(p,d,march_params.a,P,N);"
"if(.0<n)"
"{"
"if(world_toggle)q=max(dot(light_direction,N),.0)*mix(vec3(.3,.6,.9),vec3(1),smoothstep(-24.,9.,P.g))+pow(max(dot(d,reflect(light_direction,N)),.0),7.)*.11;"
"else"
"{"
"e=raycast(P+light_direction*.5,light_direction,march_params.a*3.,q,q);"
"q=(1.-e)*(max(dot(light_direction,N),.0)*mix(vec3(.8,.6,.4),vec3(1),smoothstep(-24.,9.,P.g))+pow(max(dot(d,reflect(light_direction,N)),.0),7.)*.11);"
"}"
"r=P-uniform_array[8];"
"e=destruction+.5-length(r);"
"if(0<e)q+=vec3((dot(Q(P*.009),normalize(r))*.1)+.1,-.05,-.05)*smoothstep(.0,.5,e);"
"}"
"vec3 s=mix(vec3(.9,.8,.8),vec3(.8,.8,.9),d.g*111.*.02)*(dot(Q(p*.006+d*.1),d)*smoothstep(-.2,.5,-d.g)*.2+.8);"
"if(world_toggle)n=smoothstep(.0,.4,n);"
"output_color=vec4(mix(mix(s,vec3(1),pow(max(dot(d,light_direction),.0),7.)),q,n),1.)-(int(gl_FragCoord.y*.5)%2+.1)*(max(max(smoothstep(.98,1.,uniform_array[7].g),smoothstep(-.02*uniform_array[9].r,.0,-uniform_array[7].g)*uniform_array[9].r),.1)+destruction*.02)*dot(c,c);"
"r=p;"
"e=C(r,d,uniform_array[8],destruction+.2);"
"if(.0<e)output_color.rgb-=clamp(1.-(dot(r-p,r-p)-dot(P-p,P-p))*.003,.0,1.)*(1.-pow(e/destruction,5))*(dot(Q((r-uniform_array[8])*.009),d)*.1+.9);"
"}"
#endif
"";
