using FlaxEngine;
using System;
using System.Collections.Generic;

namespace FlaxEditor.Surface
{
    /// <summary>
    /// The Visject Surface implementation for PLCT graphs.
    /// </summary>
    /// <seealso cref="VisjectSurface" />
    [HideInEditor]
    public class PLCTGraphSurface : VisjectSurface
    {
        /// <inheritdoc />
        public PLCTGraphSurface(IVisjectSurfaceOwner owner, Action onSave, FlaxEditor.Undo undo = null, SurfaceStyle style = null, List<GroupArchetype> groups = null)
    : base(owner, onSave, undo, style, groups)
        {
        }
    }
}
