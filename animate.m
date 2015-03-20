files = sort(readdir("./"));
graphics_toolkit gnuplot

for ii = 1:numel(files)
  if (length(regexp (files{ii}, "^.*out\\d+\\.dat")) == 0)
    continue;
  endif

  data = dlmread(files{ii}, " ");

  figure(1, "visible", "off");
  plot3(data(:, 1), data(:, 2), data(:, 3), "*", "markersize", 1);
  view([20+0.1*ii 30]);
  axis([-5 5 -5 5 -5 5]);
  axis("off");
  set(gcf, "color", 'black');
  sleep(1e-200);
  saveas(1, ["img" num2str(ii, "%5.5d") ".jpg"]);
endfor
